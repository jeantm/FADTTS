#include "Processing.h"

const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing()
{
}

QMap<QString, bool> Processing::GenerateMatlabInputFiles( QMap<QString, bool> selectedInputFiles, QString selectedSubjectListFile,
                                   int subjectCovariatesColumnId, QMap<int, QString> selectedCovariates, QString currentOutputDir )
{
    m_selectedInputFiles = selectedInputFiles;
    QStringList selectedSubjectList = GetSelectedSubjectList( selectedSubjectListFile );


    QMap<QString, bool>::ConstIterator iterSelectedInputFile = selectedInputFiles.begin();
    while( iterSelectedInputFile != selectedInputFiles.end() )
    {
        QStringList inputFileSubjects = GetSubjectListFromInputFile( iterSelectedInputFile.key(), subjectCovariatesColumnId );
        QFile matlabInputFile( currentOutputDir + "/" +
                       QFileInfo( QFile( iterSelectedInputFile.key() ) ).fileName().split( "." ).first() +
                       "_M.txt" );
        if( matlabInputFile.open( QIODevice::WriteOnly ) )
        {
            QTextStream tsM( &matlabInputFile );
            QStringList rowData;

            QFile file( iterSelectedInputFile.key() );
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
                    if( selectedSubjectList.contains( currentSubject ) && inputFileSubjects.count( currentSubject ) == 1 )
                    {
                        QMap<int, QString>::ConstIterator iterCovariate = selectedCovariates.begin();
                        while( iterCovariate != selectedCovariates.constEnd() )
                        {
                            int covID = iterCovariate.key();
                            if( covID != -1 && covID != subjectCovariatesColumnId )
                            {
                                rowData << data.at( r ).at( covID );
                            }
                            ++iterCovariate;
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
                    QString currentSubject = data.at( 0 ).at( c );
                    if( ( c == 0 || ( selectedSubjectList.contains( currentSubject ) && inputFileSubjects.count( currentSubject ) == 1 ) ) )
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
            matlabInputFile.flush();
            matlabInputFile.close();

            m_matlabInputFiles.insert( matlabInputFile.fileName(), iterSelectedInputFile.value() );
        }
        ++iterSelectedInputFile;
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

QStringList Processing::GetSubjectListFromInputFile( QString subjectListFile, int subjectCovariatesColumnId )
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

QStringList Processing::GetRefSubjectListFromSelectedInputFiles( QMap<QString, bool> selectedInputFiles, int subjectCovariatesColumnId )
{
    QStringList refSubjectList;
    QMap<QString, bool>::ConstIterator iterSelectedInputFile = selectedInputFiles.begin();
    while( iterSelectedInputFile != selectedInputFiles.end() )
    {
        QStringList currentSubjectList = GetSubjectListFromInputFile( iterSelectedInputFile.key(), subjectCovariatesColumnId );
        foreach( QString subject, currentSubjectList )
        {
            if( !refSubjectList.contains( subject ) )
            {
                refSubjectList.append( subject );
            }
        }
        ++iterSelectedInputFile;
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
