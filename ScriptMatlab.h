#ifndef SCRIPTMATLAB_H
#define SCRIPTMATLAB_H

#include <QString>
#include <QDir>
#include <QMap>
#include <QTextStream>
#include <QDate>
#include <QTime>

#include "para_Model_FADTTS.h"
#include "soft_Model_FADTTS.h"

class ScriptMatlab
{
public :
   // Constructor
   ScriptMatlab();

   // Set Functions

   void GenerateMatlabScript( QString fiberName, QStringList selectedPrefixes, QMap<QString, bool> matlabInputFiles,
                              QMap<int,QString> selectedCovariates, int nbrPermutation, QString outputDir );


   void InitScriptMatlab();

   void SetHeader();

   void SetFiberName( QString fiberName );

   void SetDiffusionProperties( QStringList selectedPrefixes );

   void SetNbrPermutation( int nbrPermutation );

   void SetCovariatesList( QMap<int, QString> selectedCovariates );

   void SetInputFiles( QMap<QString, bool> matlabInputFiles );


private :
   static const QString m_csvSeparator;

   QString m_script;
};


#endif // SCRIPTMATLAB_H
