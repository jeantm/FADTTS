#include "Processing.h"
#include <iostream>

#include <QDebug>

const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing( QObject *parent ) :
    QObject( parent )
{
}


/*****************************************************/
/***************** Public  Functions *****************/
/*****************************************************/
bool Processing::IsMatrixDimensionOK( const QList<QStringList> fileData )
{
    int rowDataSize = fileData.first().count();
    foreach (QStringList rowData,  fileData)
    {
        if( rowDataSize != rowData.count() )
        {
            return false;
        }
    }
    return true;
}

bool Processing::IsCovariateFile( const QStringList fileData )
{
    bool ok;
    foreach( QString data, fileData )
    {
        if( data.endsWith( '"' ) )
        {
            data.chop( 1 );
        }
        if( data.startsWith( '"' ) )
        {
            data.remove( 0, 1 );
        }
        data.toFloat( &ok );
        if( !ok )
        {
            return !ok;
        }
    }

    return !ok;
}


QStringList Processing::GetSubjectsFromInputFile( QList<QStringList> dataInInputFile, int covariateFileSubjectColumnID )
{
    QStringList subjectList;
    QList<QStringList> data = dataInInputFile;
    int nbRows = data.count();
    int nbColumns = data.first().count();

    if( IsCovariateFile( data.at( 1 ) ) )
    {
        for( int r = 1; r < nbRows; r++ )
        {
            subjectList.append( data.at( r ).at( covariateFileSubjectColumnID ) );
        }
    }
    else
    {
        for( int c = 1; c < nbColumns; c++ )
        {
            subjectList.append( data.first().at( c ) );
        }
    }
    subjectList.sort();

    return subjectList;
}

QStringList Processing::GetAllSubjectsList( QMap<QString, QStringList> allSubjects )
{
    /** Create a subject list of reference.
     *  This list is either a file provided by the user or automatically generated
     *  with the selected input files' subjects **/

    QStringList allSubjectsList;
    QMap<QString, QStringList>::ConstIterator iter = allSubjects.begin();
    while( iter != allSubjects.end() )
    {
        allSubjectsList.append( iter.value() );
        ++iter;
    }
    allSubjectsList.removeDuplicates();

    return allSubjectsList;
}

QMap<QString, QStringList> Processing::GetAllSubjectsFromSelectedInputFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap )
{
    QMap<QString, QStringList> allSubjectsFromSelectedInputFiles;
    QMap<QString, QCheckBox*>::ConstIterator iterCheckBoxMap = checkBoxMap.begin();
    QMap<QString, QStringList >::ConstIterator iterSubjectsList = subjectsMap.begin();
    while( iterCheckBoxMap != checkBoxMap.constEnd() )
    {
        if( iterCheckBoxMap.value()->isChecked() )
        {
            ( allSubjectsFromSelectedInputFiles[iterSubjectsList.key()] ).append( iterSubjectsList.value() );

        }
        ++iterCheckBoxMap;
        ++iterSubjectsList;
    }

    return allSubjectsFromSelectedInputFiles;
}

QMap< QString, QMap<QString, bool> > Processing::SortSubjects( const QStringList allSubjectsList, const QMap<QString, QStringList> allSubjects )
{
    QMap< QString, QMap<QString, bool> > sortedSubjects;
    foreach( QString subject, allSubjectsList )
    {
        QMap<QString, QStringList >::ConstIterator iterSelectedSubjects = allSubjects.begin();
        while( iterSelectedSubjects != allSubjects.constEnd() )
        {
            ( ( sortedSubjects[subject] )[iterSelectedSubjects.key()] ) = iterSelectedSubjects.value().contains( subject ) ? true : false;
            ++iterSelectedSubjects;
        }
    }

    return sortedSubjects;
}

void Processing::AssignSortedSubject( const QMap< QString, QMap<QString, bool> > sortedSubjects, QStringList& matchedSubjects,
                                      QMap<QString, QStringList >& unMatchedSubjects )
{
    QMap< QString, QMap<QString, bool> >::ConstIterator iterSortedSubjects = sortedSubjects.begin();
    while( iterSortedSubjects != sortedSubjects.end() )
    {
        bool subjectInAll = true;
        QMap<QString, bool>::ConstIterator it = iterSortedSubjects.value().begin();
        while( it != iterSortedSubjects.value().end() )
        {
            subjectInAll = subjectInAll && it.value();
            ++it;
        }

        if( subjectInAll )
        {
            matchedSubjects.append( iterSortedSubjects.key() );
        }
        else
        {
            QMap<QString, bool>::ConstIterator it = iterSortedSubjects.value().begin();
            while( it != iterSortedSubjects.value().end() )
            {
                if( it.value() )
                {
                    ( unMatchedSubjects[iterSortedSubjects.key()] ).append( it.key() );
                }
                ++it;
                unMatchedSubjects[iterSortedSubjects.key()].sort();
            }
        }
        ++iterSortedSubjects;
    }
}


QList<QStringList> Processing::GetDataFromFile( QString filePath )
{
    QFile file( filePath );
    file.open( QIODevice::ReadOnly );

    QTextStream ts( &file );
    QList<QStringList> fileData;
    while( !ts.atEnd() )
    {
        fileData << ts.readLine().split( m_csvSeparator );
    }
    file.close();

    return fileData;
}

QMap<int, QString> Processing::GetCovariatesFromFileData( QList<QStringList> dataCovariateFile, int covariateFileSubjectColumnID )
{
    QMap<int, QString> covariates;
    int nbrColumns = dataCovariateFile.first().count();
    for( int c = 0; c < nbrColumns; ++c )
    {
        if( c != covariateFileSubjectColumnID )
        {
            covariates.insert( c, dataCovariateFile.first().at( c ) );
        }
    }

    return covariates;
}

QMap< QPair< int, QString >, bool> Processing::GenerateMatlabInputFiles( QMap< QPair< int, QString >, bool > selectedInputFiles, QMap< int, QString > propertySelected,
                                                                         QString selectedSubjectFile, int covariateFileSubjectColumnId, QMap<int, QString> selectedCovariates,
                                                                         QString outputDir, QString fiberName )
{
    QMap< QPair< int, QString >, bool> matlabInputFiles;
    QStringList selectedSubjectList = GetSelectedSubjects( selectedSubjectFile );

    QMap< QPair< int, QString >, bool>::ConstIterator iterSelectedInputFile = selectedInputFiles.begin();
    QMap< int, QString >::ConstIterator iterPropertySelected = propertySelected.begin();
    while( iterSelectedInputFile != selectedInputFiles.end() )
    {
        QString fileName = iterSelectedInputFile.key().second;
        QFile matlabInputFile( outputDir + "/" + fiberName + "_" + iterPropertySelected.value().toUpper() + "_RawData.csv" );
        matlabInputFile.open( QIODevice::WriteOnly );
        QTextStream tsM( &matlabInputFile );
        QStringList rowData;

        QList<QStringList> fileData = GetDataFromFile( fileName );

        int nbRows = fileData.count();
        int nbColumns = fileData.first().count();

        QStringList subjectAdded;
        if( IsCovariateFile( fileData.at( 1 ) ) )
        {
            for( int r = 0; r < nbRows; r++ )
            {
                QString currentSubject = fileData.at( r ).at( covariateFileSubjectColumnId );

                rowData.clear();
                if( !subjectAdded.contains( currentSubject ) )
                {
                    if( r == 0 )
                    {
                        rowData << currentSubject;

                        QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
                        while( iterCovariate != selectedCovariates.constEnd() )
                        {
                            int covID = iterCovariate.key();
                            if( covID != -1 )
                            {
                                rowData << fileData.at( r ).at( covID );
                            }
                            ++iterCovariate;
                        }
                    }

                    if( selectedSubjectList.contains( currentSubject ) )
                    {
                        rowData << currentSubject;

                        QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
                        while( iterCovariate != selectedCovariates.constEnd() )
                        {
                            int covID = iterCovariate.key();
                            if( covID != -1 )
                            {
                                rowData << fileData.at( r ).at( covID );
                            }
                            ++iterCovariate;
                        }
                    }

                    if( !rowData.isEmpty() )
                    {
                        tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
                    }
                    subjectAdded.append( currentSubject );
                }
            }
        }
        else
        {
            QList<int> subjectID;
            for( int c = 0; c < nbColumns; c++ )
            {
                QString currentSubject = fileData.first().at( c );
                if( ( c == 0 || ( selectedSubjectList.contains( currentSubject ) && !subjectAdded.contains( currentSubject ) ) ) )
                {
                    subjectID.append( c );
                    subjectAdded.append( currentSubject );
                }
            }

            for( int r = 0; r < nbRows; ++r )
            {
                rowData.clear();
                foreach ( int id, subjectID )
                {
                    rowData << fileData.at( r ).at( id );
                }
                tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
            }
        }
        matlabInputFile.flush();
        matlabInputFile.close();

        QPair< int, QString > currentPair;
        currentPair.first = iterSelectedInputFile.key().first;
        currentPair.second = matlabInputFile.fileName();
        matlabInputFiles.insert( currentPair, iterSelectedInputFile.value() );

        ++iterSelectedInputFile;
        ++iterPropertySelected;
    }

    return matlabInputFiles;
}


/*****************************************************/
/***************** Private Functions *****************/
/*****************************************************/
QStringList Processing::GetSelectedSubjects( QString selectedSubjectFile )
{
    QStringList selectedSubjectList;
    QFile file( selectedSubjectFile );
    if( !file.fileName().isNull() )
    {
        if( file.open( QIODevice::ReadOnly ) )
        {
            QTextStream tsSelectedSubjectList( &file );
            while( !tsSelectedSubjectList.atEnd() )
            {
                selectedSubjectList.append( tsSelectedSubjectList.readLine() );
            }
            file.close();
        }
    }

    return selectedSubjectList;
}

QStringList Processing::GetRefSubjectsFromSelectedInputFiles( QMap< QPair< int, QString >, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID )
{
    QStringList refSubjectList;
    QMap< QPair< int, QString >, QList<QStringList> >::ConstIterator iterDataInSelectedFile = dataInSelectedInputFiles.begin();
    while( iterDataInSelectedFile != dataInSelectedInputFiles.end() )
    {
        QStringList currentSubjectList = GetSubjectsFromInputFile( iterDataInSelectedFile.value(), covariateFileSubjectColumnID );
        foreach( QString subject, currentSubjectList )
        {
            if( !refSubjectList.contains( subject ) )
            {
                refSubjectList.append( subject );
            }
        }
        ++iterDataInSelectedFile;
    }
    refSubjectList.removeDuplicates();

    return refSubjectList;
}
