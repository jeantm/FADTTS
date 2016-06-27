#include "Processing.h"

#include <iostream>
#include <cmath>

#include <QDebug>


const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing( QObject *parent ) :
    QObject( parent )
{
}


/*****************************************************/
/***************** Public  Functions *****************/
/*****************************************************/
QList< QStringList > Processing::GetDataFromFile( QString filePath )
{
    QFile file( filePath );
    QList< QStringList > fileData;
    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream ts( &file );
        QStringList  tempRows;

        /** Read all the file line by line **/
        while( !ts.atEnd() )
        {
            tempRows.append( ts.readLine() );
        }
        file.close();

        /** If only one line read, check for the carriage return character "\r" **/
        if(  tempRows.size() == 1 )
        {
            tempRows =  tempRows.first().split( "\r" );
        }
        tempRows.removeAll( "" );

        /** For each line (== row), dispatch data in columns **/
        foreach( QString dataRow,  tempRows )
        {
            QStringList tempRow = dataRow.split( m_csvSeparator );
            tempRow.removeAll( "" );

            /** remove quotes if needed **/
            foreach( QString data, tempRow)
            {
                if( data.endsWith( '"' ) )
                {
                    data.chop( 1 );
                }
                if( data.startsWith( '"' ) )
                {
                    data.remove( 0, 1 );
                }
            }

            fileData.append( tempRow );
        }
    }

    return fileData;
}

bool Processing::IsMatrixDimensionOK( const QList< QStringList >& data )
{
    if( data.isEmpty() )
    {
        return false;
    }
    else
    {
        /** Check if the matrix data has the same number of columns for each row **/
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

bool Processing::IsSubMatrix( const QList< QStringList >& data )
{
    bool ok;
    QStringList dataSecondRow = data.at( 1 );
    foreach( QString d, dataSecondRow )
    {
        d.toFloat( &ok );
        if( !ok && d != "-nan" )
        {
            return !ok;
        }
    }

    return !ok;
}

bool Processing::AreDuplicatesFound( const QList< QStringList >& data )
{
    bool isSubMatrixData = IsSubMatrix( data );
    QStringList subjects = isSubMatrixData ? Transpose( data ).first() : data.first();
    int duplicateSubjectsFound = 0;

    duplicateSubjectsFound = subjects.removeDuplicates();

    return duplicateSubjectsFound != 0;
}

QStringList Processing::GetNANSubjects( const QList< QStringList >& faData, const QStringList& matchedSubjects )
{
    QList< QStringList > tempData = Transpose( faData );
    QStringList nanSubjects;

    foreach( QStringList row, tempData )
    {
        int i = 1;
        bool found = false;
        QString currentSubject = row.first();

        if( matchedSubjects.contains( currentSubject ) )
        {
            while( i < row.size() && found == false )
            {
                if( row.at( i ) == "nan" || row.at( i ) == "-nan" )
                {
                    nanSubjects.append( row.first() );
                    found = true;
                }
                i++;
            }
        }
    }

    return nanSubjects;
}

void Processing::NANToZeros( QList< QList< double > >& faData )
{
    for( int i = 0; i < faData.size(); i++ )
    {
        for( int j = 0; j < faData.first().size(); j++ )
        {
            if( isnan( faData.at( i ).at( j ) ) )
            {
                faData[ i ][ j ] = 0;
            }
        }
    }
}


QStringList Processing::GetSubjectsFromFileList( QString filePath )
{
    QStringList subjectList;

    QFile file( filePath );
    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream ts( &file );

        /** Read all the file line by line **/
        while( !ts.atEnd() )
        {
            subjectList.append( ts.readLine() );
        }
        file.close();

        /** If only one line read, check for the carriage return character "\r" **/
        if(  subjectList.size() == 1 )
        {
            subjectList =  subjectList.first().split( "\r" );
        }
        subjectList.removeAll( "" );
        subjectList.sort();
    }

    return subjectList;
}

QStringList Processing::GetSubjectsFromData(const QList< QStringList >& data, int subjectColumnID )
{
    QStringList subjectList;
    int nbRows = data.count();
    int nbColumns = data.first().count();

    if( IsSubMatrix( data ) )
    {
        for( int row = 1; row < nbRows; row++ )
        {
            subjectList.append( data.at( row ).at( subjectColumnID ) );
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

QMap< int, QString > Processing::GetCovariatesFromData( QList< QStringList > data, int subjectColumnID )
{
    QMap< int, QString > covariates;
    int nbrColumns = data.first().count();
    for( int column = 0; column < nbrColumns; ++column )
    {
        if( column != subjectColumnID )
        {
            covariates.insert( column, data.first().at( column ) );
        }
    }

    return covariates;
}

QList< QStringList > Processing::Transpose( const QList< QStringList >& rawData )
{
    QList< QStringList > rawDataTransposed;
    for( int j = 0; j < rawData.first().size(); j++ )
    {
        QStringList rowData;
        for( int i = 0; i < rawData.size(); i++ )
        {
            rowData.append( rawData.at( i ).at( j ) );
        }
        rawDataTransposed.append( rowData );
    }

    return rawDataTransposed;
}

bool Processing::RemoveUnmatchedSubjects( QList< QStringList >& rawData, QStringList& subjects, const QStringList& matchedSubjects )
{
    QList< QStringList > tempData;
    for( int i = 0; i < rawData.size(); i++ )
    {
        QString currentSubject = rawData.at( i ).first();

        if( matchedSubjects.contains( currentSubject ) )
        {
            tempData.append( rawData.at( i ) );
        }
        else
        {
            subjects.removeAll( currentSubject );
        }
    }

    rawData.clear();
    rawData = tempData;

    QStringList tempSubjects = subjects;
    tempSubjects.sort();

    return tempSubjects == matchedSubjects;
}

QList< double > Processing::QStringListToDouble( const QStringList& rowData )
{
    QList< double > rowDataDouble;
    foreach ( QString data, rowData )
    {
        rowDataDouble.append( data.toDouble() );
    }
    return rowDataDouble;
}

QList< QList< double > > Processing::DataToDouble( const QList< QStringList >& data )
{
    QList< QList< double > > dataDouble;
    foreach ( QStringList rowData, data )
    {
        dataDouble.append( QStringListToDouble( rowData ) );
    }
    return dataDouble;
}

QList< double > Processing::GetMean( const QList< QList< double > >& rawDataDouble, int shift )
{
    QList< double > mean;

    int nbrPoints = rawDataDouble.first().size();
    int nbrSubjects = rawDataDouble.size();

    for( int i = shift; i < nbrPoints; i++ )
    {
        double currentMean = 0;
        for( int j = 0; j < nbrSubjects; j++ )
        {
            currentMean += rawDataDouble.at( j ).at( i );
        }
        currentMean = currentMean / nbrSubjects;
        mean.append( currentMean );
    }

    return mean;
}

double Processing::ApplyPearsonCorrelation( const QList< double >& currentLine, const QList< double >& mean, int shift )
{
    double pearsonCorrelation = 1.0;
    int nbrPoints = mean.size();

    if( nbrPoints > 1)
    {
        double sumMean = 0;
        for( int i = 0; i < nbrPoints; i++ )
        {
            sumMean += mean.at( i );
        }

        double sumMeanSquare = 0;
        for( int i = 0; i < nbrPoints; i++ )
        {
            sumMeanSquare += mean.at( i ) * mean.at( i );
        }

        double sumCurrentLine = 0;
        for( int i = shift; i < nbrPoints + shift; i++ )
        {
            sumCurrentLine += currentLine.at( i );
        }

        double sumCurrentLineSquare = 0;
        for( int i = shift; i < nbrPoints + shift; i++ )
        {
            sumCurrentLineSquare += currentLine.at( i ) * currentLine.at( i );
        }

        double meanXcurrentLine = 0;
        for( int i = 0; i < nbrPoints; i++ )
        {
            meanXcurrentLine += mean.at( i ) * currentLine.at( i + shift );
        }

        pearsonCorrelation = ( meanXcurrentLine - ( sumMean * sumCurrentLine ) / nbrPoints ) /
                std::sqrt( ( sumMeanSquare - std::pow( sumMean, 2 ) / nbrPoints ) * ( sumCurrentLineSquare - std::pow( sumCurrentLine, 2 ) / nbrPoints ) );
    }

    return pearsonCorrelation;
}


QStringList Processing::GetAllSubjects( const QMap< int, QStringList >& subjectsMap )
{
    /** Return a list of all subjects found in the input files provided **/

    QStringList allSubjects;
    QMap< int, QStringList >::ConstIterator iterSubjectList = subjectsMap.cbegin();
    while( iterSubjectList != subjectsMap.cend() )
    {
        allSubjects.append( iterSubjectList.value() );
        ++iterSubjectList;
    }
    allSubjects.removeDuplicates();
    allSubjects.sort();

    return allSubjects;
}

QMap< int, QStringList > Processing::GetSubjectsFromSelectedFiles( const QMap< int, bool >& diffusionPropertiesCheckState, const QMap< int, QStringList >& subjectsMap )
{
    QMap< int, QStringList > subjectsFromSelectedFiles;
    QMap< int, bool >::ConstIterator iterProperty = diffusionPropertiesCheckState.cbegin();
    QMap< int, QStringList >::ConstIterator iterSubjects = subjectsMap.cbegin();
    while( iterProperty != diffusionPropertiesCheckState.cend() )
    {
        if( iterProperty.value() )
        {
            ( subjectsFromSelectedFiles[iterSubjects.key()] ).append( iterSubjects.value() );

        }
        ++iterProperty;
        ++iterSubjects;
    }

    return subjectsFromSelectedFiles;
}

QMap< QString, QMap< int, bool > > Processing::SortSubjects( const QStringList &subjects, const QMap< int, QStringList >& subjectsMap )
{
    QMap< QString, QMap< int, bool > > sortedSubjects;
    foreach( QString subject, subjects )
    {
        QMap< int, QStringList >::ConstIterator iterSubjects = subjectsMap.cbegin();
        while( iterSubjects != subjectsMap.cend() )
        {
            ( ( sortedSubjects[subject] )[iterSubjects.key()] ) = iterSubjects.value().contains( subject ) ? true : false;
            ++iterSubjects;
        }
    }

    return sortedSubjects;
}

void Processing::AssignSortedSubject( const QMap<QString, QMap< int, bool > >& sortedSubjects, QStringList& matchedSubjects, QMap<QString, QList< int > >& unMatchedSubjects )
{
    /** We go through all the sorted subjects **/
    QMap< QString, QMap< int, bool > >::ConstIterator iterSortedSubjects = sortedSubjects.cbegin();
    while( iterSortedSubjects != sortedSubjects.cend() )
    {
        bool subjectInAll = true;

        /** For each subjects, we check if it is contained in the required files/list **/
        QMap< int, bool >::ConstIterator iterSubject = iterSortedSubjects.value().cbegin();
        while( iterSubject != iterSortedSubjects.value().cend() )
        {
            subjectInAll = subjectInAll && iterSubject.value();
            ++iterSubject;
        }

        /** If the subject is found in all of the required files/list, it is added to the matched subjects QStringList **/
        if( subjectInAll )
        {
            matchedSubjects.append( iterSortedSubjects.key() );
        }
        /** Otherwise the subject is added to the unmatched subjects QMap with the index of the diffusion property file where
         *  the subject has been found in and, if necessary the index of the loaded subject list (-1) **/
        else
        {
            QMap< int, bool >::ConstIterator iterSubject = iterSortedSubjects.value().cbegin();
            while( iterSubject != iterSortedSubjects.value().cend() )
            {
                if( iterSubject.value() )
                {
                    ( unMatchedSubjects[iterSortedSubjects.key()] ).append( iterSubject.key() );
                }
                ++iterSubject;
                unMatchedSubjects[iterSortedSubjects.key()];
            }
        }
        ++iterSortedSubjects;
    }
}


QMap< int, QString > Processing::GenerateMatlabInputs( QString outputDir, QString fiberName, const QMap< int, QString >& inputs, const QMap< int, QString >& properties,
                                                       const QMap< int, QString >& covariates, int subjectColumnID, const QStringList& subjects, int startProfile, int endProfile )
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
        QFile matlabInput( outputDir + "/" + fiberName + "_RawData_" + iterProperty.value().toUpper() + ".csv" );
        matlabInput.open( QIODevice::WriteOnly );
        QTextStream tsM( &matlabInput );

        QList< QStringList > data = GetDataFromFile( iterInput.value() );
        int nbRows = data.count();
        int nbColumns = data.first().count();

        QStringList rowData;
        QStringList subjectProcessed;
        if( IsSubMatrix( data ) )
        {
            /** File is SubMatrix -> subject data stored by row.
             *  Subjects are all in the column subjectColumnID. **/

            for( int row = 0; row < nbRows; row++ )
            {
                rowData.clear();

                QString currentSubject = data.at( row ).at( subjectColumnID );
                /** If subject is not already stored. **/
                if( !subjectProcessed.contains( currentSubject ) )
                {

                    /** 1st row is where all covariate names ares.
                     *  If current subject is among the required subjects or on 1st column**/
                    if( row == 0 || subjects.contains( currentSubject ) )
                    {
                        /** If row == 0 --> Only required covariates are kept. 'Intercept' (-1) is ignored.
                         *  If subjects.contains( currentSubject ) --> Data in the required covariates columns is kept **/
                        QMap< int, QString >::ConstIterator iterCovariate = covariates.cbegin();
                        while( iterCovariate != covariates.cend() )
                        {
                            int covariateID = iterCovariate.key();
                            if( covariateID != -1 )
                            {
                                rowData.append( data.at( row ).at( covariateID ) );
                            }
                            ++iterCovariate;
                        }
                    }

                    /** If not empty, row is added to the new file **/
                    if( !rowData.isEmpty() )
                    {
                        rowData.prepend( currentSubject );
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
            QList< int > columnIndex;
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

            /** Adding 1st row **/
            foreach ( int index, columnIndex )
            {
                rowData << data.at( 0 ).at( index );
            }
            tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;

            /** Look through the 'original' and copy data to the new one **/
            int profileFirstIndex = startProfile == -1 ? 1 : startProfile + 1;
            int profileLastIndex = endProfile == -1 ? nbRows : endProfile + 2;
            for( int row = profileFirstIndex; row < profileLastIndex; ++row )
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


void Processing::ApplyQCThreshold_noGUI( const QList< QStringList >& rawData, bool useAtlas, QStringList& matchedSubjects, QStringList& qcThresholdFailedSubject, const double& qcThreshold )
{
    QStringList subjectsCorrelated, subjectsNotCorrelated;
    QStringList subjects = rawData.first();

    /*** Transpose ***/
    QList< QStringList > rawDataTransposed = Transpose( rawData );

    /*** Remove unmatched subjects ***/
    RemoveUnmatchedSubjects( rawDataTransposed, subjects, matchedSubjects );

    /*** To Double ***/
    QList< QList< double > > rawDataDouble = DataToDouble( rawDataTransposed );

    /*** Get Ref ***/
    QList< double > ref;
    if( useAtlas )
    {
        if( rawData.first().last().contains( "atlas", Qt::CaseInsensitive ) )
        {
            for( int i = 1; i < rawData.size(); i++ )
            {
                ref.append( rawData.at( i ).last().toDouble() );
            }
        }
    }
    else
    {
        ref = GetMean( rawDataDouble, 1 );
    }

    /*** Apply QCThreshold ***/
    for( int i = 1; i < rawDataDouble.size(); i++ )
    {
        double pearsonCorrelation = ApplyPearsonCorrelation( rawDataDouble.at( i ), ref, 1 );

        if( pearsonCorrelation < qcThreshold )
        {
            subjectsNotCorrelated.append( rawDataTransposed.at( i ).first() );
        }
        else
        {
            subjectsCorrelated.append( rawDataTransposed.at( i ).first() );
        }
    }

    matchedSubjects = subjectsCorrelated;
    qcThresholdFailedSubject = subjectsNotCorrelated;
}

