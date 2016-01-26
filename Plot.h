#ifndef PLOT_H
#define PLOT_H

#include "Processing.h"

#include <QDir>
#include <QFileDialog>

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
#include <vtkChartLegend.h>
#include <vtkRenderWindow.h>


class Plot : public QObject
{
    Q_OBJECT

public:
    explicit Plot( QObject *parent = 0 );


    void SetQVTKWidget( QVTKWidget *qvtkWidget );

    void InitPlot( QString directory, QString fibername );

    void ResetDataFile();

    void ResetPlot();


    void SelectPlot( QString plotSelected );

    void SelectProperty( QString property );

    void SelectCovariate( QString covariateSelected );


    void SetLinesToDisPlay( const QMap< int, QPair< QString, QPair< bool, QString > > > currentLinesForDisplay );

    void SetMarkerType( QString markerType );

    void SetAlpha( double alpha );

    void SetTitle( QString title, bool isBold, bool isItalic, double fontSize );

    void SetDefaultTitle();

    void SetAxis( QString xName, QString yName, bool isBold, bool isItalic,
                  bool isYMinSet, double yMin, bool isYMaxSet, double yMax );

    void SetDefaultAxis();

    void SetLegend( QString position );


    bool DisplayVTKPlot();


    void UpdateCovariatesNames( QMap< int, QString > newCovariatesNames );


public slots:
    void SavePlot();

signals:
    void PlotsUsed( const QStringList& );

    void AllPropertiesUsed( const QStringList& );

    void AllCovariatesUsed( const QMap< int, QString >& );

    void CovariatesAvailableForPlotting( const QMap< int, QString >& );


private:
    static QMap< QString, QList < int > > InitColorMap()
    {
        QMap< QString, QList < int > > newColorMap;
        newColorMap.insert( "Red", QList< int >() << 255 << 0 << 0 );
        newColorMap.insert( "Green", QList< int >() << 0 << 128 << 0 );
        newColorMap.insert( "Blue", QList< int >() << 0 << 0 << 255 );
        newColorMap.insert( "Purple", QList< int >() << 128 << 0 << 128 );
        newColorMap.insert( "Yellow", QList< int >() << 255 << 255 << 0 );
        newColorMap.insert( "Cyan", QList< int >() << 0 << 255 << 255 );
        newColorMap.insert( "Magenta", QList< int >() << 255 << 0 << 255 );
        newColorMap.insert( "Grey", QList< int >() << 128 << 128 << 128 );
        newColorMap.insert( "Maroon", QList< int >() << 128 << 0 << 0 );
        newColorMap.insert( "Lime", QList< int >() << 0 << 255 << 0 );
        newColorMap.insert( "Teal", QList< int >() << 0 << 128 << 128 );
        newColorMap.insert( "Navy", QList< int >() << 255 << 0 << 0 );
        newColorMap.insert( "Black", QList< int >() << 0 << 0 << 0 );

        return newColorMap;
    }

//    static QMap< QString, int[3] > InitColorMap()
//    {
//        QMap< QString, int[3] > newColorMap;
//        int red[3] = { 255 , 0 , 0 };
//        int green[3] = { 0 , 128 , 0 };
//        int blue[3] = { 0 , 0 , 255 };
//        int purple[3] = { 128 , 0 , 128 };
//        int yellow[3] = { 255 , 255 , 0 };
//        int cyan[3] = { 0 , 255 , 255 };
//        int magenta[3] = { 255 , 0 , 255 };
//        int grey[3] = { 128 , 128 , 128 };
//        int maroon[3] = { 128 , 0 , 0 };
//        int lime[3] = { 0 , 255 , 0 };
//        int teal[3] = { 0 , 128 , 128 };
//        int navy[3] = { 255 , 0 , 0 };
//        int black[3] = { 0 , 0 , 0 };
//        newColorMap.insert( "Red", red );
//        newColorMap.insert( "Green", green );
//        newColorMap.insert( "Blue", blue );
//        newColorMap.insert( "Purple", purple );
//        newColorMap.insert( "Yellow", yellow );
//        newColorMap.insert( "Cyan", cyan );
//        newColorMap.insert( "Magenta", magenta );
//        newColorMap.insert( "Grey", grey );
//        newColorMap.insert( "Maroon", maroon );
//        newColorMap.insert( "Lime", lime );
//        newColorMap.insert( "Teal", teal );
//        newColorMap.insert( "Navy", navy );
//        newColorMap.insert( "Black", black );

//        return newColorMap;
//    }


    static const QMap< QString, QList < int > > m_allColors;

    Processing m_process;

    QVTKWidget *m_qvtkWidget;
    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkChartXY> m_chart;
    vtkSmartPointer<vtkChartLegend> m_legend;
    QMap< int, vtkPlot* > m_line;


    QMap< QString, QList < QList < double > > > m_csvRawData, m_csvBeta,
    m_csvPostHocLpvalue, m_csvPostHocFDRLpvalue;

    QMap< int, QPair< QString, QPair< bool, QString > > > m_linesToDisplay;

    QMap< int, QString > m_covariatesNoIntercept, m_allCovariates, m_binaryCovariates;

    QList< QList < int > > m_lineColors;

    QList < QStringList > m_csvCovariate;

    QList < QList < double > > m_csvOmnibusLpvalue, m_csvOmnibusFDRLpvalue, m_ordinate;

    QList< double > m_abscissa;

    QStringList m_csvRawDataFiles, m_csvBetaFiles, m_csvOmnibusLpvalueFiles, m_csvOmnibusFDRLpvalueFiles,
    m_csvPostHocFDRLpvalueFiles, m_properties, m_lineNames;

    QPair< bool, double > m_yMin, m_yMax;

    QString m_matlabDirectory, m_directory, m_plotSelected, m_propertySelected, m_covariateSelected,
    m_title, m_fibername, m_xName, m_yName;

    double m_alpha;

    int m_nbrPlot, m_nbrPoint, m_markerType;

    bool m_yMinChecked, m_yMaxChecked,
    m_isBinaryCovariatesSend, m_isAllCovariatesSend, m_isCovariatesNoInterceptSend;


    void SortFilesByProperties( QString directory, QStringList files, QMap< QString, QList < QList < double > > > &data );

    void TransposeData( QList < QList < double > > &data, int firstRow, int firstColumn );

    void TransposeDataInQMap( QMap< QString, QList<QList<double> > > &data, int firstRow, int firstColumn );

    QList < double > QStringListToDouble( QStringList rowData );

    QList < QList < double > > DataToDouble( QList < QStringList > data );


    void SetRawData();

    void SetBeta();

    void SetOmnibusLpvalue( QStringList OmnibusLpvalueFiles, QList < QList < double > > &OmnibusLpvaluesData );

    void SetPostHocFDRLpvalue();


    void GetRawDataFiles();

    void GetBetaFiles();

    void GetOmnibusLpvalueFiles();

    void GetOmnibusFDRLpvalueFiles();

    void GetPostHocFDRLpvalueFiles();


    void SetPlots();

    void SetProperties();

    void SetCovariates();

    void SetAbscissa();


    void ResetLoadData();


    void SeparateBinary( QList< QList < double > > &temp0Bin, QList< QList < double > > &temp1Bin );

    void GetMeanAndStdDv( QList< QList < double > > tempBin, QList < double > &tempMean, QList < double > &tempStdDv );

    void ProcessRawStats( QList< QList < double > > tempBin, QList < double > &tempBinMean, QList < double > &tempBinUp, QList < double > &tempBinDown );

    void LoadLinesToDisplay( QList< QList < double > > &ordinate );

    QList< QList < double > > Tolog10( QList< QList < double > > data );


    QList< QList < double > > LoadRawData();

    QList< QList < double > > LoadRawStats();

    QList< QList < double > > LoadBetas();

    QList< QList < double > > LoadBetaByCovariate();

    QList< QList < double > > LoadOmnibusLpvalues( QList<QList<double> > omnibusLpvalues );

    QList< QList < double > > LoadPostHocFDRLpvalues();

    bool LoadData();


    void AddEntriesRawData( vtkSmartPointer<vtkTable> &table );

    void AddEntriesRawStats( vtkSmartPointer<vtkTable> &table );

    void AddEntriesByPropertiesOrCovariates( vtkSmartPointer<vtkTable> &table );

    void AddEntries( vtkSmartPointer<vtkTable> &table );


    void SetData( vtkSmartPointer<vtkTable> &table );


    void InitLines();

    void AddLineSigBetas( vtkSmartPointer<vtkTable> table, bool betaDisplayedByProperties, bool isOmnibus, int i, int incrementColor );

    void AddLineRawData( vtkSmartPointer<vtkTable> table );

    void AddLineRawStats( vtkSmartPointer<vtkTable> table );

    void AddLineBetas( vtkSmartPointer<vtkTable> table, bool isSigBeta, bool betaDisplayedByProperties, bool isOmnibus );

    void AddLineLPvalue( vtkSmartPointer<vtkTable> table );

    void AddLines( vtkSmartPointer<vtkTable> table );


    QList< double > GetyMinMax();

    void SetyMinMax();

    void SetChartProperties();
};

#endif // PLOT_H
