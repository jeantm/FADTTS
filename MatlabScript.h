#ifndef MATLABSCRIPT_H
#define MATLABSCRIPT_H

#include <QResource>
#include <QDate>
#include <QTime>
#include <QString>
#include <QTextStream>
#include <QDir>
#include <QMap>
#include <QDebug>

#include "para_Model_FADTTS.h"
#include "soft_Model_FADTTS.h"

#ifndef FADTTS_VERSION
#define FADTTS_VERSION "unknown"
#endif

#ifndef FADTTS_CONTRIBUTOR
#define FADTTS_CONTRIBUTOR "unknown"
#endif

class MatlabScript
{
public :
   MatlabScript();


   QString GenerateMatlabScript( QString outputDir, QString fiberName, QStringList selectedPrefixes, QMap<QString, bool> matlabInputFiles,
                                 QMap<int,QString> selectedCovariates, int nbrPermutations, bool omnibus, bool postHoc );

   void ResetScript();


private :
   static const QString m_csvSeparator;

   QString m_scriptPath, m_script;


   void InitMatlabScript();

   void SetHeader();

   void SetFiberName( QString fiberName );

   void SetDiffusionProperties( QStringList selectedPrefixes );

   void SetNbrPermutations( int nbrPermutation );

   void SetCovariatesList( QMap<int, QString> selectedCovariates );

   void SetInputFiles( QMap<QString, bool> matlabInputFiles );

   void SetOmnibus( bool omnibus );

   void SetPostHoc( bool postHoc );
};


#endif // MATLABSCRIPT_H
