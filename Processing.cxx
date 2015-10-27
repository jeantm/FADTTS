#include "Processing.h"

const QString Processing::m_csvSeparator = QLocale().groupSeparator();

Processing::Processing()
{
}


/***************** Running Process *****************/
void Processing::RunScript( QString matlabExe, QString scriptPath )
{
    qDebug() << "In RunScript()";
    qDebug() << scriptPath;

    QProcess process;
    QStringList arguments;
    arguments << "-nojvm" << "-nosplash" << QString( "-r " + scriptPath );
    process.start( matlabExe, arguments );
    process.waitForFinished(-1);
}


bool Processing::IsCOMPFile( const QStringList strList )
{
    bool ok;
    foreach( QString str, strList )
    {
        if( str.endsWith( '"' ) )
        {
            str.chop( 1 );
        }
        if( str.startsWith( '"' ) )
        {
            str.remove( 0, 1 );
        }
        str.toFloat( &ok );
        if( !ok )
        {
            return !ok;
        }
    }
    return !ok;
}

QStringList Processing::GetSelectedSubjectList( QString selectedSubjectListFile )
{
    QStringList selectedSubjectList;
    QFile file( selectedSubjectListFile );
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

QMap<QString, bool> Processing::GenerateMatlabInputFiles( QMap<QString, bool> selectedInputFiles, QString selectedSubjectListFile,
                                                          int subjectCovariatesColumnId, QMap<int, QString> selectedCovariates,
                                                          QString outputDir, QString fiberName )
{
    QMap<QString, bool> matlabInputFiles;
    QStringList selectedSubjectList = GetSelectedSubjectList( selectedSubjectListFile );

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

        QList<QStringList> data = GetDataFromFile( fileName );

        int nbRows = data.count();
        int nbColumns = data.at( 0 ).count();

        QStringList subjectAdded;
        if( IsCOMPFile( data.at( 1 ) ) )
        {
            for( int r = 0; r < nbRows; r++ )
            {
                QString currentSubject = data.at( r ).at( subjectCovariatesColumnId );

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
                                rowData << data.at( r ).at( covID );
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
                                rowData << data.at( r ).at( covID );
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
            for( int c = 0; c < nbColumns; c++ )
            {
                QString currentSubject = data.at( 0 ).at( c );
                if( ( c == 0 || ( selectedSubjectList.contains( currentSubject ) && !subjectAdded.contains( currentSubject ) ) ) )
                {
                    subjectID.append( c );
                    subjectAdded.append( currentSubject );
                }
                else
                {
                    /***************** WARNING POPUP *****************/
                    // not in subject lisst ref or appears twice in datafile
                }
            }

            for( int r = 0; r < nbRows; ++r )
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

        matlabInputFiles.insert( encoding.append( matlabInputFile.fileName() ), iterSelectedInputFile.value() );

        ++iterSelectedInputFile;
    }
    return matlabInputFiles;
}


/****************** Other Processes ******************/
QStringList Processing::GetSubjectListFromInputFile( QList<QStringList> dataInInputFile, int subjectCovariatesColumnId )
{
    QStringList subjectList;

    QList<QStringList> data = dataInInputFile;
    int nbRows = data.count();
    int nbColumns = data.at( 0 ).count();

    if( IsCOMPFile( data.at( 1 ) ) )
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
    subjectList.sort();

    return subjectList;
}

QStringList Processing::GetRefSubjectListFromSelectedInputFiles( QMap<QString, QList<QStringList> > dataInSelectedInputFiles, int subjectCovariatesColumnId )
{
    QStringList refSubjectList;
    QMap<QString, QList<QStringList> >::ConstIterator iterDataInSelectedFile = dataInSelectedInputFiles.begin();
    while( iterDataInSelectedFile != dataInSelectedInputFiles.end() )
    {
        QStringList currentSubjectList = GetSubjectListFromInputFile( iterDataInSelectedFile.value(), subjectCovariatesColumnId );
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

QStringList Processing::GetRefSubjectList( const QString subjectListFilePath, QMap<QString, QList<QStringList> > dataInSelectedFiles, int subjectCovariatesColumnId )
{
    /** Create a subject list of reference.
     *  This list is either a file provided by the user or automatically generated
     *  with the selected input files' subjects **/
    QFile subjectFile( subjectListFilePath );
    QStringList refSubjectList;
    if( QFileInfo( subjectFile ).fileName().isNull() )
    {
        refSubjectList = GetRefSubjectListFromSelectedInputFiles( dataInSelectedFiles, subjectCovariatesColumnId );
    }
    else
    {
        if( !subjectFile.open( QIODevice::ReadOnly ) )
        {
            refSubjectList = GetRefSubjectListFromSelectedInputFiles( dataInSelectedFiles, subjectCovariatesColumnId );
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

QMap< QString, QMap<QString, bool> > Processing::SortSubjectInInputFile( const QStringList refList, const QMap<QString, QStringList> subjectMap )
{
    QMap< QString, QMap<QString, bool> > checkedSubject;
    QStringListIterator itRef( refList );
    while( itRef.hasNext() )
    {
        QString subjRef = QString( itRef.next() );

        QMap<QString, QStringList >::ConstIterator iterSubjectList = subjectMap.begin();
        while( iterSubjectList != subjectMap.constEnd() )
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

void Processing::AssignSortedSubject( const QMap< QString, QMap<QString, bool> > checkedSubject, QStringList& matchedSubjects,
                                      QMap<QString, QStringList >& unMatchedSubjects )
{
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


QList<QStringList> Processing::GetDataFromFile( QString fileName )
{
    QFile file( fileName );
    file.open( QIODevice::ReadOnly );

    QTextStream ts( &file );
    QList<QStringList> data;
    while( !ts.atEnd() )
    {
        data << ts.readLine().split( m_csvSeparator );
    }

    file.close();

    //    qDebug() << data;

    return data;
}
