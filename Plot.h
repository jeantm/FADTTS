#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>

#include <QVTKWidget.h>
//#include <cmath>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkPlotPoints.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkPen.h>


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void InitPlot( QString directory );

    void ResetDataFile();

    void ResetPlot();


    void SelectPlot( QString plotSelected );

    void SelectOutcome( QString outcome );    

    void SelectCovariate( QString covariateSelected );

    void SetTitle( QString title );

    void SetxName( QString xName );

    void SetyName( QString yName );

    void SetYMin( bool yMinChecked, double yMin );

    void SetYMax( bool yMaxChecked, double yMax );


    void DisplayVTKPlot();


public slots:
    void SavePlot();

    void AlphaValueChanged( const double& newAlpha );


signals:
    void OutcomeUsed( const QStringList&  );

    void CovariateUsed( const QMap< int, QString >&  );


private:
    QVTKWidget *m_qvtkWidget;

    Processing m_process;

    vtkSmartPointer<vtkContextView> m_view;

    vtkSmartPointer<vtkChartXY> m_chart;

    vtkSmartPointer<vtkTable> m_table, m_tableSigBetas;

    QMap< int, vtkPlot* > m_line;

    QMap< QString, QList < QList < double > > > m_csvRawData, m_csvBeta,
    m_csvPostHocLpvalue, m_csvPostHocFDRLpvalue;

    QList < QList < double > > m_csvOmnibusLpvalue, m_csvOmnibusFDRLpvalue;

    QList < QStringList > m_csvCovariate;

    QStringList m_outcomeUsed;

    QMap< int, QString > m_covariateNoIntercept, m_allCovariate, m_binaryCovariate;

    QList< double > m_abscissa;

    QList< QList < double > > m_ordinate;

    QStringList m_csvRawDataFile, m_csvBetaFile, m_csvOmnibusLpvalueFile, m_csvOmnibusFDRLpvalueFile,
    m_csvPostHocLpvalueFile, m_csvPostHocFDRLpvalueFile;

    QString m_matlabOutputDir, m_directory,
    m_plotSelected, m_outcomeSelected, m_covariateSelected,
    m_title, m_xName, m_yName;

    double m_yMin, m_yMax, m_yMinGiven, m_yMaxGiven, m_alpha;

    int m_indexColumn, m_nbrPlot, m_nbrPoint;

    bool m_isCovariateBinary, m_yMinChecked, m_yMaxChecked;



    void SortFiles( QString directory, QStringList files, QMap< QString, QList < QList < double > > > &data );

    void TransposeData( QList < QList < double > > &data, int iMin, int jMin );

    void Transpose( QMap< QString, QList < QList < double > > > &data, int iMin, int jMin );

    QList < double > DataToDouble( QStringList rowData );

    QList < QList < double > > ToDouble( QList < QStringList > data );


    void SetRawData( QStringList files, QMap< QString, QList < QList < double > > > &data );

    void SetBeta(QStringList files, QMap< QString, QList<QList<double> > > &data );

    void SetOmnibusLpvalue( QStringList files );

    void SetOmnibusFDRLpvalue( QStringList files );

    void SetPostHocFDRLpvalue( QStringList files, QMap< QString, QList < QList < double > > > &data );


    void GetRawDataFiles();

    void GetBetaFiles();

    void GetOmnibusLpvalueFiles();

    void GetOmnibusFDRLpvalueFiles();

    void GetPostHocFDRLpvalueFiles();


    void SetOutcomes();

    void SetCovariates();

    void SetAbscissa();


    void ResetLoadData();


    void SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin );

    void GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv );

    void ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown );

    QList< QList < double > > Tolog10( QList< QList < double > > data );


    QList< QList < double > > LoadRawData();

    QList< QList < double > > LoadRawStats();

    QList< QList < double > > LoadBetas();

    QList< QList < double > > LoadBetaByCovariate();

    QList< QList < double > > LoadOmnibusLpvalues();

    QList< QList < double > > LoadOmnibusFDRLpvalues();

    QList< QList < double > > LoadPostHocFDRLpvalues();

    bool LoadData();


    void AddEntryRawData();

    void AddEntryRawStats();

    void AddEntryCovariate( QMap< int, QString > covariates );

    void AddEntryOutcome();

    void AddEntry();


    void SetData();


    void InitLines();

    void AddLineRawData();

    void AddLineRawStats();

    void AddLineRawBetas();

    void AddLineOmnibusPvalue();

    void AddLinePostHocPvalue();

    void AddLineOmnibusFDRSigBetaByProperty();

    void AddLineOmnibusFDRSigBetaByCovariate();

    void AddLinePostHocFDRSigBetaByProperty();

    void AddLinePostHocFDRSigBetaByCovariate();

    void AddLines();


    void SetDefaultTitle();

    void FindyMinMax();

    void SetChartProperties();
};

#endif // PLOT_H
