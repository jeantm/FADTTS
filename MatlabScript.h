#ifndef MATLABSCRIPT_H
#define MATLABSCRIPT_H

#include "para_Model_FADTTS.h"
#include "soft_Model_FADTTS.h"

#include <iostream>

#include <QLocale>
#include <QResource>
#include <QTime>
#include <QMap>
#include <QDir>
#include <QTextStream>


#ifndef FADTTS_TITLE
#define FADTTS_TITLE "Unknown Title"
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


   void InitMatlabScript();

   void SetHeader();

   void SetMVCMPath( QString mvcmPath );

   void SetFiberName( QString fiberName );

   void SetDiffusionProperties( QStringList selectedPrefixes );

   void SetNbrPermutation( int nbrPermutation );

   void SetCovariates( QMap<int, QString> selectedCovariates );

   void SetInputFiles( QMap< QPair< int, QString >, bool> matlabInputFiles );

   void SetOmnibus( bool omnibus );

   void SetPostHoc( bool postHoc );

   QString GenerateMatlabFiles( QString matlabOutputDir, QString fiberName, int nbrPermutations );


private :
   static const QString m_csvSeparator;

   QString m_matlabScript;
};


#endif // MATLABSCRIPT_H
