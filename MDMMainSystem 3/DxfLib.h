#ifndef DXFLIB_H
#define DXFLIB_H

#include <QSet>
#include <QString>
#include <QList>
#include <QString>
#include <QFile>
#include <QMap>
#include <QPointF>
#include <QPoint>
#include <QtMath>
#include "lib/dxf/dl_dxf.h"
#include "lib/dxf/dl_creationadapter.h"
#include "lib/dxf/dl_attributes.h"
#include "lib/dxf/dl_codes.h"
#include "lib/dxf/dl_entities.h"
#include "lib/dxf/dl_exception.h"
#include "lib/dxf/dl_global.h"
#include "lib/dxf/dl_writer.h"
#include "lib/dxf/dl_writer_ascii.h"

class DxfLib : public DL_CreationAdapter
{
public:
    //    explicit EntitiesDatas();
    virtual void addPoint(const DL_PointData &d);
    virtual void addLine(const DL_LineData &d);
    virtual void addArc(const DL_ArcData &d);
    virtual void addCircle(const DL_CircleData &d);
    virtual void addEllipse(const DL_EllipseData &d);
    virtual void addText(const DL_TextData &d);
    virtual void addDimAngular(const DL_DimensionData &d, const DL_DimAngularData &d1);
    //    virtual EntDimLinear(const DL_DimensionData &d, const DL_DimLinearData &d1){
    //        qDebug(________________add deim linear";
    //    }
    virtual void addMText(const DL_MTextData &d);
    virtual void addXLine(const DL_XLineData &);
    virtual void addRay(const DL_RayData &);
    virtual void addPolyline(const DL_PolylineData &d);
    virtual void addSpline(const DL_SplineData &);
    virtual void addComment(const std::string &);
    virtual void addVertex(const DL_VertexData &d);
    virtual void addLayer(const DL_LayerData &d);
    virtual void addXDataString(int, const std::string &);
    virtual void addXRecord(const std::string &);
    virtual void addTrace(const DL_TraceData &);
    virtual void endEntity();
    virtual void endSequence();
    virtual void setAttributes(const DL_Attributes &attrib);
    virtual void addHatch(const DL_HatchData &d);
    virtual void addHatchEdge(const DL_HatchEdgeData &d);

    // struct poly2{
    //     QList<DL_PolylineData> polyli;
    //    QList<DL_VertexData> verticelist;
    //};

    //    virtual void EntitiesDatas::att
    //poly2 dllm;

    QSet<QString> layers;
    QList<DL_PointData> points;
    //QList<DL_LineData> lines;//
    //QList<DL_ArcData> arcs;
    // QList<DL_CircleData> circles;//
    //QList<DL_EllipseData> ellipses;
    QList<DL_TextData> texts;
    QList<DL_DimAngularData> dimangulars;
    QList<DL_DimLinearData> dimlines;
    QList<DL_DimRadialData> radias;
    //QList<DL_VertexData> vertices;//
    //QList<DL_PolylineData> polylines;//
    QList<DL_MTextData> mtexts;//
    QList<DL_HatchData> hatches;//
    QList<DL_HatchEdgeData> hatchedges;//
    QList<QString> hatchcolors;

    QMultiMap<QString,DL_ArcData> arcs;
    QMultiMap<QString,DL_LineData> lines;
    QMultiMap<QString,DL_SplineData> splines;
    QMultiMap<QString,DL_CircleData> circles;
    QMultiMap<QString,DL_VertexData> vertices;
    QMultiMap<QString,DL_PolylineData> polylines;
    QMultiMap<QString,DL_EllipseData> ellipses;
    // QMultiMap<QString, poly2> polylines;
};

#endif // DXFLIB_H
