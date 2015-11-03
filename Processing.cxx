#include "Processing.h"

const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing()
{
}


/***************** Running Process *****************/
void Processing::RunScript( QString matlabExe, QString matlabScript )
{
    qDebug() << "In RunScript()";
    qDebug() << matlabScript;

    QProcess process;
    QStringList arguments;
    arguments << "-nojvm" << "-nosplash" << QString( "-r " + matlabScript );
    process.start( matlabExe, arguments );
    process.waitForFinished(-1);
}


bool Processing::IsMatrixDimensionOK( const QList<QStringList> fileData )
{
    int rowDataSize = fileData.at( 0 ).count();
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

QMap<QString, bool> Processing::GenerateMatlabInputFiles( QMap<QString, bool> selectedInputFiles, QString selectedSubjectFile,
                                                          int covariateFileSubjectColumnId, QMap<int, QString> selectedCovariates,
                                                          QString outputDir, QString fiberName )
{
    QMap<QString, bool> matlabInputFiles;
    QStringList selectedSubjectList = GetSelectedSubjects( selectedSubjectFile );

    QMap<QString, bool>::ConstIterator iterSelectedInputFile = selectedInputFiles.begin();
    while( iterSelectedInputFile != selectedInputFiles.end() )
    {
        QString encoding = iterSelectedInputFile.key().split( "?" ).first() + "?";
        QString fileName = iterSelectedInputFile.key().split( "?" ).last();
        QFile matlabInputFile( outputDir + "/" + fiberName + "_" +
                               QFileInfo( QFile( fileName ) ).fileName().split( "." ).first() + "_MatlabInput.csv" );
        matlabInputFile.open( QIODevice::WriteOnly );
        QTextStream tsM( &matlabInputFile );
        QStringList rowData;

        QList<QStringList> fileData = GetDataFromFile( fileName );

        int nbRows = fileData.count();
        int nbColumns = fileData.at( 0 ).count();

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
                QString currentSubject = fileData.at( 0 ).at( c );
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

        matlabInputFiles.insert( encoding.append( matlabInputFile.fileName() ), iterSelectedInputFile.value() );

        ++iterSelectedInputFile;
    }

    return matlabInputFiles;
}


/****************** Other Processes ******************/
QStringList Processing::GetSubjectsFromInputFile( QList<QStringList> dataInInputFile, int covariateFileSubjectColumnID )
{
    QStringList subjectList;
    QList<QStringList> data = dataInInputFile;
    int nbRows = data.count();
    int nbColumns = data.at( 0 ).count();

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
            subjectList.append( data.at( 0 ).at( c ) );
        }
    }
    subjectList.sort();

    return subjectList;
}

QStringList Processing::GetRefSubjectsFromSelectedInputFiles( QMap<QString, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID )
{
    QStringList refSubjectList;
    QMap<QString, QList<QStringList> >::ConstIterator iterDataInSelectedFile = dataInSelectedInputFiles.begin();
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

QStringList Processing::GetRefSubjects( const QString subjectFilePath, QMap<QString, QList<QStringList> > dataInSelectedInputFiles, int covariateFileSubjectColumnID )
{
    /** Create a subject list of reference.
     *  This list is either a file provided by the user or automatically generated
     *  with the selected input files' subjects **/
    QFile subjectFile( subjectFilePath );
    QStringList refSubjectList;
    if( QFileInfo( subjectFile ).fileName().isNull() )
    {
        refSubjectList = GetRefSubjectsFromSelectedInputFiles( dataInSelectedInputFiles, covariateFileSubjectColumnID );
    }
    else
    {
        if( !subjectFile.open( QIODevice::ReadOnly ) )
        {
            refSubjectList = GetRefSubjectsFromSelectedInputFiles( dataInSelectedInputFiles, covariateFileSubjectColumnID );
        }
        else
        {
            QTextStream ts( &subjectFile );
            while( !ts.atEnd() )
            {
                refSubjectList.append( ts.readLine() );
            }
            subjectFile.close();
        }
    }
    refSubjectList.removeDuplicates();

    return refSubjectList;
}


QMap<QString, QStringList> Processing::GetAllSubjectsFromSelectedInputFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap )
{
    QMap<QString, QStringList> selectedSubjectList;
    QMap<QString, QCheckBox*>::ConstIterator iterCheckBoxMap = checkBoxMap.begin();
    QMap<QString, QStringList >::ConstIterator iterSubjectsList = subjectsMap.begin();
    while( iterCheckBoxMap != checkBoxMap.constEnd() )
    {
        if( iterCheckBoxMap.value()->isChecked() )
        {
            ( selectedSubjectList[iterSubjectsList.key()] ).append( iterSubjectsList.value() );

        }
        ++iterCheckBoxMap;
        ++iterSubjectsList;
    }

    return selectedSubjectList;
}

QMap< QString, QMap<QString, bool> > Processing::SortSubjects( const QStringList refSubjects, const QMap<QString, QStringList> selectedSubjects )
{
    QMap< QString, QMap<QString, bool> > checkedSubject;
    QStringListIterator itRef( refSubjects );
    while( itRef.hasNext() )
    {
        QString subjRef = QString( itRef.next() );

        QMap<QString, QStringList >::ConstIterator iterSubjectList = selectedSubjects.begin();
        while( iterSubjectList != selectedSubjects.constEnd() )
        {
            if( iterSubjectList.value().contains( subjRef ) )
            {
                ( ( checkedSubject[subjRef] )[iterSubjectList.key()] ) = true;
            }
            else
            {
                ( ( checkedSubject[subjRef] )[iterSubjectList.key()] ) = false;
            }
            ++iterSubjectList;
        }
    }

    return checkedSubject;
}

void Processing::AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubjects, QStringList& matchedSubjects,
                                      QMap<QString, QStringList >& unMatchedSubjects )
{
    QMap< QString, QMap<QString, bool> >::ConstIterator iterCheckedSubject = checkedSubjects.begin();
    while( iterCheckedSubject != checkedSubjects.constEnd() )
    {
        bool subInAll = true;
        QMap<QString, bool>::ConstIterator it = iterCheckedSubject.value().begin();
        while( it != iterCheckedSubject.value().constEnd() )
        {
            subInAll = subInAll && it.value();
            ++it;
        }

        if( subInAll )
        {
            matchedSubjects.append( iterCheckedSubject.key() );
        }
        else
        {
            QMap<QString, bool>::ConstIterator it = iterCheckedSubject.value().begin();
            while( it != iterCheckedSubject.value().constEnd() )
            {
                if( !it.value() && !( unMatchedSubjects[iterCheckedSubject.key()] ).contains( "refList" ) )
                {
                    ( unMatchedSubjects[iterCheckedSubject.key()] ).append( QObject::tr( "refList" ) );
                }
                if( it.value() )
                {
                    ( unMatchedSubjects[iterCheckedSubject.key()] ).append( it.key() );
                }
                ++it;
                unMatchedSubjects[iterCheckedSubject.key()].sort();
            }
        }
        ++iterCheckedSubject;
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
    int nbrColumns = dataCovariateFile.at( 0 ).count();
    for( int c = 0; c < nbrColumns; ++c )
    {
        if( c != covariateFileSubjectColumnID )
        {
            covariates.insert( c, dataCovariateFile.at( 0 ).at( c ) );
        }
    }

    return covariates;
}
