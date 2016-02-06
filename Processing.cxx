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
bool Processing::IsMatrixDimensionOK( const QList<QStringList> data )
{
    if( data.isEmpty() )
    {
        return false;
    }
    else
    {
        int rowDataSize = data.first().count();
        foreach ( QStringList rowData,  data )
        {
            if( rowDataSize != rowData.count() )
            {
                return false;
            }
        }
        return true;
    }
}

bool Processing::IsCovariateFile( const QStringList file )
{
    bool ok;
    foreach( QString data, file )
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


QStringList Processing::GetSubjectsFromData( QList<QStringList> data, int covariateColumnID )
{
    QStringList subjectList;
    int nbRows = data.count();
    int nbColumns = data.first().count();

    if( IsCovariateFile( data.at( 1 ) ) )
    {
        for( int row = 1; row < nbRows; row++ )
        {
            subjectList.append( data.at( row ).at( covariateColumnID ) );
        }
    }
    else
    {
        for( int column = 1; column < nbColumns; column++ )
        {
            subjectList.append( data.first().at( column ) );
        }
    }
    subjectList.sort();

    return subjectList;
}


QStringList Processing::GetAllSubjects( QMap<QString, QStringList> subjectsMap )
{
    /** Return a list of all subjects found in the input files provided **/

    QStringList allSubjects;
    QMap<QString, QStringList>::ConstIterator iterSubjectList = subjectsMap.cbegin();
    while( iterSubjectList != subjectsMap.cend() )
    {
        allSubjects.append( iterSubjectList.value() );
        ++iterSubjectList;
    }
    allSubjects.removeDuplicates();

    return allSubjects;
}

QMap<QString, QStringList> Processing::GetSubjectsFromSelectedFiles( const QMap<QString, QCheckBox*> checkBoxMap, const QMap<QString, QStringList > subjectsMap )
{
    QMap<QString, QStringList> subjectsFromSelectedFiles;
    QMap<QString, QCheckBox*>::ConstIterator iterCheckBoxMap = checkBoxMap.cbegin();
    QMap<QString, QStringList >::ConstIterator iterSubjects = subjectsMap.cbegin();
    while( iterCheckBoxMap != checkBoxMap.cend() )
    {
        if( iterCheckBoxMap.value()->isChecked() )
        {
            ( subjectsFromSelectedFiles[iterSubjects.key()] ).append( iterSubjects.value() );

        }
        ++iterCheckBoxMap;
        ++iterSubjects;
    }

    return subjectsFromSelectedFiles;
}

QMap< QString, QMap<QString, bool> > Processing::SortSubjects( const QStringList subjects, const QMap<QString, QStringList> subjectsMap )
{
    QMap< QString, QMap<QString, bool> > sortedSubjects;
    foreach( QString subject, subjects )
    {
        QMap<QString, QStringList >::ConstIterator iterSubjects = subjectsMap.cbegin();
        while( iterSubjects != subjectsMap.cend() )
        {
            ( ( sortedSubjects[subject] )[iterSubjects.key()] ) = iterSubjects.value().contains( subject ) ? true : false;
            ++iterSubjects;
        }
    }

    return sortedSubjects;
}

void Processing::AssignSortedSubject( const QMap< QString, QMap<QString, bool> > sortedSubjects, QStringList& matchedSubjects,
                                      QMap<QString, QStringList >& unMatchedSubjects )
{
    QMap< QString, QMap<QString, bool> >::ConstIterator iterSortedSubjects = sortedSubjects.cbegin();
    while( iterSortedSubjects != sortedSubjects.cend() )
    {
        bool subjectInAll = true;
        QMap<QString, bool>::ConstIterator iterSubject = iterSortedSubjects.value().cbegin();
        while( iterSubject != iterSortedSubjects.value().cend() )
        {
            subjectInAll = subjectInAll && iterSubject.value();
            ++iterSubject;
        }

        if( subjectInAll )
        {
            matchedSubjects.append( iterSortedSubjects.key() );
        }
        else
        {
            QMap<QString, bool>::ConstIterator iterSubject = iterSortedSubjects.value().cbegin();
            while( iterSubject != iterSortedSubjects.value().cend() )
            {
                if( iterSubject.value() )
                {
                    ( unMatchedSubjects[iterSortedSubjects.key()] ).append( iterSubject.key() );
                }
                ++iterSubject;
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

QMap<int, QString> Processing::GetCovariatesFromData( QList<QStringList> data, int covariateColumnID )
{
    QMap<int, QString> covariates;
    int nbrColumns = data.first().count();
    for( int column = 0; column < nbrColumns; ++column )
    {
        if( column != covariateColumnID )
        {
            covariates.insert( column, data.first().at( column ) );
        }
    }

    return covariates;
}


QMap< int, QString > Processing::GenerateMatlabInputs( QString outputDir, QString fiberName,
                                                       QMap< int, QString > inputs, QMap< int, QString > properties,
                                                       QMap<int, QString> covariates, QStringList subjects, int covariateColumnID )
{
    /** Files with standard names are created in the output directory.
     *  Based on the input type (AD/RD/MD/FA/SubMatrix), the covariates, and the subjects,
     *  data is selected and copied from the 'original' files into the new ones, leaving
     *  the originals inchanged **/
    QMap< int, QString > matlabInputs;

    QMap< int, QString >::ConstIterator iterInput = inputs.cbegin();
    QMap< int, QString >::ConstIterator iterProperty = properties.cbegin();
    while( iterInput != inputs.cend() )
    {
        QFile matlabInput( outputDir + "/" + fiberName + "_" + iterProperty.value().toUpper() + "_RawData.csv" ); // check if modification works; iterProperty.value().toUpper()
        matlabInput.open( QIODevice::WriteOnly );
        QTextStream tsM( &matlabInput );

        QList<QStringList> data = GetDataFromFile( iterInput.value() );
        int nbRows = data.count();
        int nbColumns = data.first().count();

        QStringList rowData;
        QStringList subjectProcessed;
        if( IsCovariateFile( data.at( 1 ) ) )
        {
            /** File is Covariate -> subject data stored by row.
             *  Subjects are all in the column covariateColumnID. **/

            for( int row = 0; row < nbRows; row++ )
            {
                rowData.clear();

                QString currentSubject = data.at( row ).at( covariateColumnID );

                /** If subject is not already stored. **/
                if( !subjectProcessed.contains( currentSubject ) )
                {

                    /** 1st row is where all covariate names are. **/
                    if( row == 0 )
                    {
                        rowData << currentSubject;

                        /** Only required covariates are kept. 'Intercept' (-1) is ignored. **/
                        QMap<int, QString>::ConstIterator iterCovariate = covariates.cbegin();
                        while( iterCovariate != covariates.cend() )
                        {
                            int covariateID = iterCovariate.key();
                            if( covariateID != -1 )
                            {
                                rowData << data.at( row ).at( covariateID );
                            }
                            ++iterCovariate;
                        }
                    }

                    /** If current subject is among the required subjects **/
                    if( subjects.contains( currentSubject ) )
                    {
                        rowData << currentSubject;

                        /** Data in the required covariates columns is kept  **/
                        QMap<int, QString>::ConstIterator iterCovariate = covariates.cbegin();
                        while( iterCovariate != covariates.cend() )
                        {
                            int covariateID = iterCovariate.key();
                            if( covariateID != -1 )
                            {
                                rowData << data.at( row ).at( covariateID );
                            }
                            ++iterCovariate;
                        }
                    }

                    /** If not empty, row is added to the new file **/
                    if( !rowData.isEmpty() )
                    {
                        tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
                    }

                    /** Update the list of subjects added to the new file to avoid duplicates **/
                    subjectProcessed.append( currentSubject );
                }
            }
        }
        else
        {
            /** File is either AD, RD, MD or FA -> subject data stored by column.
             *  Subjects are all in the 1s row.
             *  1st column is Arc Length **/

            /** Index of the required columns are saved.
             *  The column index is saved only if the subject linked to this index is
             *  among the required subjects and is not a duplicate.
             *  0 is always saved. **/
            QList<int> columnIndex;
            columnIndex.append( 0 );
            for( int column = 1; column < nbColumns; column++ )
            {
                QString currentSubject = data.first().at( column );
                if( ( subjects.contains( currentSubject ) && !subjectProcessed.contains( currentSubject ) ) )
                {
                    columnIndex.append( column );
                    subjectProcessed.append( currentSubject );
                }
            }

            /** Look through the 'original' and copy data to the new one **/
            for( int row = 0; row < nbRows; ++row )
            {
                rowData.clear();
                foreach ( int index, columnIndex )
                {
                    rowData << data.at( row ).at( index );
                }
                tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
            }
        }
        matlabInput.flush();
        matlabInput.close();

        /** Update QMap where the path of the new files is stored **/
        matlabInputs.insert( iterInput.key(), matlabInput.fileName() );

        ++iterInput;
        ++iterProperty;
    }

    return matlabInputs;
}
