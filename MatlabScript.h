#ifndef MATLABSCRIPT_H
#define MATLABSCRIPT_H

//#include "para_Model_FADTTS.h"
//#include "soft_Model_FADTTS.h"

#include <iostream>

#include <QLocale>
#include <QResource>
#include <QTime>
#include <QMap>
#include <QDir>
#include <QTextStream>


#ifndef FADTTS_TITLE
#define FADTTS_TITLE "Unknown"
#endif

#ifndef FADTTS_VERSION
#define FADTTS_VERSION "unknown"
#endif

#ifndef FADTTS_CONTRIBUTORS
#define FADTTS_CONTRIBUTORS "unknown"
#endif

#ifndef FADTTS_DESCRIPTION
#define FADTTS_DESCRIPTION "No description available"
#endif


class MatlabScript
{
public :
   MatlabScript();


   void InitMatlabScript( QString matlabOutputDir, QString matlabScriptName );


   void SetHeader();


   void SetNbrCompThreads( bool isRunOnSystem , int nbrComp );

   void SetMVCMPath( QString mvcmPath );


   void SetFiberName( QString fiberName );

   void SetDiffusionProperties( QStringList selectedPrefixes );

   void SetInputFiles( QMap<int, QString> matlabInputFiles );

   void SetCovariates( QMap<int, QString> selectedCovariates );


   void SetNbrPermutation( int nbrPermutation );

   void SetOmnibus( bool omnibus );

   void SetPostHoc( bool postHoc );

   void SetConfidenceBandsThreshold( double confidenceBandsThreshold );

   void SetPvalueThreshold( double pvalueThreshold );


   QString GenerateMatlabFiles();


private :
   static const QString m_csvSeparator;

   QString m_matlabScript, m_matlabOutputDir, m_matlabScriptName;


   void GenerateMyFDR();
};


#endif // MATLABSCRIPT_H
