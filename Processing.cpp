#include "Processing.h"

const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing()
{
}

QStringList Processing::GenerateMatlabInputFiles( QStringList selectedInputFiles, QString selectedSubjectListFile,
                                   int subjectCovariatesColumnId, QList<int> selectedCovariates, QString currentFileOutputDir )
{
    m_selectedInputFiles = selectedInputFiles;
    QStringList selectedSubjectList = GetSelectedSubjectList( selectedSubjectListFile );
    foreach (QString inputFile, selectedInputFiles)
    {
        QStringList subject = GetSubjectListFromDataFile( inputFile, subjectCovariatesColumnId );
        QFile matlabFile( currentFileOutputDir + "/" +
                       QFileInfo( QFile( inputFile ) ).fileName().split( "." ).first() +
                       "_M.txt" );
        if( matlabFile.open( QIODevice::WriteOnly ) )
        {
            QTextStream tsM( &matlabFile );
            QStringList rowData;

            QFile file( inputFile );
            file.open( QIODevice::ReadOnly );
            QTextStream ts( &file );
            QList<QStringList> data;
            while( !ts.atEnd() )
            {
                data << ts.readLine().split( m_csvSeparator );
            }
            file.close();
            int nbRows = data.count();
            int nbColumns = data.at( 0 ).count();

            if( nbColumns < nbRows )
            {
                for( int r = 1; r < nbRows; ++r )
                {
                    QString currentSubject = data.at( r ).at( subjectCovariatesColumnId );

                    rowData.clear();
                    if( subject.count( currentSubject ) == 1 )
                    {
                        foreach ( int covID, selectedCovariates )
                        {
                            if( covID != subjectCovariatesColumnId )
                            {
                                rowData << data.at( r ).at( covID );
                            }
                        }
                        tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
                    }
                    else
                    {
                        /***************** WARNING POPUP *****************/
                        // not in subject lisst ref or appears twice in datafile
                    }
                }
            }
            else
            {
                QList<int> subjectID;
                for( int c = 0; c < nbColumns; ++c )
                {
                    QString CurrentSubject = data.at( 0 ).at( c );
                    if( ( c == 0 || ( selectedSubjectList.contains( CurrentSubject ) && subject.count( CurrentSubject ) == 1 ) ) )
                    {
                        subjectID.append( c );
                    }
                    else
                    {
                        /***************** WARNING POPUP *****************/
                        // not in subject lisst ref or appears twice in datafile
                    }
                }

                for( int r = 1; r < nbRows; ++r )
                {
                    rowData.clear();
                    foreach ( int id, subjectID )
                    {
                        rowData << data.at( r ).at( id );
                    }
                    tsM << QObject::tr( qPrintable( rowData.join( m_csvSeparator ) ) ) << endl;
                }
            }
            matlabFile.flush();
            matlabFile.close();

            m_matlabInputFiles.append( matlabFile.fileName() );
        }
    }
    return m_matlabInputFiles;
}

QStringList Processing::GetSelectedSubjectList( QString selectedSubjectListFile )
{
    m_selectedSubjectListFile = selectedSubjectListFile;
    QFile file( selectedSubjectListFile );
    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream tsSelectedSubjectList( &file );
        while( !tsSelectedSubjectList.atEnd() )
        {
            m_selectedSubjectList.append( tsSelectedSubjectList.readLine() );
        }
        file.close();
    }
    return m_selectedSubjectList;
}

QStringList Processing::GetSubjectListFromDataFile( QString subjectListFile, int subjectCovariatesColumnId )
{
    QStringList subjectList;
    QFile file( subjectListFile );
    QList<QStringList> data;
    if( file.open( QIODevice::ReadOnly ) )
    {
        QTextStream tsSubjectList( &file );
        while( !tsSubjectList.atEnd() )
        {
            data.append( tsSubjectList.readLine().split( m_csvSeparator ) );
        }
        file.close();
        int nbRows = data.count();
        int nbColumns = data.at( 0 ).count();
        if( nbColumns < nbRows )
        {
            for( int r = 1; r < nbRows; r++ )
            {
                subjectList.append( data.at( r ).at( subjectCovariatesColumnId ) );
            }
        }
        else
        {
            for( int c = 1; c < nbColumns; c++ )
            {
                subjectList.append( data.at( 0 ).at( c ) );
            }
        }
    }

    return subjectList;
}

QStringList Processing::GetRefSubjectListFromSelectedInputFiles( QStringList selectedInputFiles, int subjectCovariatesColumnId )
{
    QStringList refSubjectList;
    foreach( QString inputFile, selectedInputFiles )
    {
        QStringList currentSubjectList = GetSubjectListFromDataFile( inputFile, subjectCovariatesColumnId );
        foreach( QString subject, currentSubjectList )
        {
            if( !refSubjectList.contains( subject ) )
            {
                refSubjectList.append( subject );
            }
        }
    }
    return refSubjectList;
}

void Processing::ResetProcessing()
{
    m_selectedSubjectListFile.clear();
    m_selectedInputFiles.clear();
    m_selectedSubjectList.clear();
    m_matlabInputFiles.clear();
}



void Processing::AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubject, QStringList& matchedSubjectList,
                                       QMap<QString, QStringList >& unMatchedSubjectList , QString subjectListFilePath )
{
    QFile subjectListFile( subjectListFilePath );
    QMap< QString, QMap<QString, bool> >::ConstIterator iterCheckedSubject = checkedSubject.begin();
    while( iterCheckedSubject != checkedSubject.constEnd() )
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
            matchedSubjectList.append( iterCheckedSubject.key() );
        }
        else
        {
            QMap<QString, bool>::ConstIterator it = iterCheckedSubject.value().begin();
            while( it != iterCheckedSubject.value().constEnd() )
            {
                if( !it.value() && !( unMatchedSubjectList[iterCheckedSubject.key()] ).contains( "refList" )
                        && ( subjectListFile.open( QIODevice::ReadOnly ) ) )
                {
                    ( unMatchedSubjectList[iterCheckedSubject.key()] ).append( QObject::tr( "refList" ) );
                }
                subjectListFile.close();
                if( it.value() )
                {
                    ( unMatchedSubjectList[iterCheckedSubject.key()] ).append( it.key() );
                }
                ++it;
            }
        }
        ++iterCheckedSubject;
    }
}



QMap< QString, QMap<QString, bool> > Processing::FindSubjectInInputFile( const QStringList refList, const QMap<QString, QStringList> subjectList )
{
    QMap< QString, QMap<QString, bool> > checkedSubject;
    QStringListIterator itRef( refList );
    while( itRef.hasNext() )
    {
        QString subjRef = QString( itRef.next().toLocal8Bit().data() );

        QMap<QString, QStringList >::ConstIterator iterSubjectList = subjectList.begin();
        while( iterSubjectList != subjectList.constEnd() )
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
