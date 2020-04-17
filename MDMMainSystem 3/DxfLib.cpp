#include "DxfLib.h"

void DxfLib::addPoint(const DL_PointData &d){
    points.append(d);
    //qDebug()<<"add point"<< d.x << "/" << d.y;
}
void DxfLib::addLine(const DL_LineData &d){
    DL_Attributes attributes = getAttributes();
    lines.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    // qDebug()<<"layer" << QString::fromStdString(attributes.getLayer());
    //qDebug()<<"add line"<<d.x1<<d.x2<<d.y1<<d.y1;

}
void DxfLib::addArc(const DL_ArcData &d){
    DL_Attributes attributes = getAttributes();
    arcs.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    // qDebug()<<"add arc";
}
void DxfLib::addCircle(const DL_CircleData &d){
    DL_Attributes attributes = getAttributes();
    circles.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    // qDebug()<<"add circle"<<d.cx<<d.cy<<d.radius;
}
void DxfLib::addEllipse(const DL_EllipseData &d){
    DL_Attributes attributes = getAttributes();
    ellipses.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    //qDebug()<<"add ellipse"<<d.cx<<d.cy;
    // qDebug()<<"add ellipse";
}
void DxfLib::addText(const DL_TextData &d){
    texts.append(d);
    //  qDebug()<<"add text";
}
void DxfLib::addDimAngular(const DL_DimensionData &d, const DL_DimAngularData &d1){
    //  qDebug()<<"add angular";
}
//    virtual DxfLib::addDimLinear(const DL_DimensionData &d, const DL_DimLinearData &d1){
//        qDebug()<<"___________________________add deim linear";
//    }
void DxfLib::addMText(const DL_MTextData &d){
    mtexts.append(d);
    //qDebug()<<"add mtext";
}
void DxfLib::addXLine(const DL_XLineData &){
    // qDebug()<<"add xline";
}
void DxfLib::addRay(const DL_RayData &){
    // qDebug()<<"add ray";
}
void DxfLib::addPolyline(const DL_PolylineData &d){
    DL_Attributes attributes = getAttributes();
    polylines.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    // qDebug()<<"add polyline data";
}
void DxfLib::addSpline(const DL_SplineData &d){
    DL_Attributes attributes = getAttributes();
    splines.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    // qDebug()<<"add spline data";
}
void DxfLib::addComment(const std::string &){
    // qDebug()<<"add comment";
}
void DxfLib::addVertex(const DL_VertexData &d){
    DL_Attributes attributes = getAttributes();
    // dllm.verticelist.append(d);
    vertices.insert(QString::fromStdString(attributes.getLayer()), d);
    layers.insert(QString::fromStdString(attributes.getLayer()));
    //qDebug()<<"add vertex";
}
void DxfLib::addLayer(const DL_LayerData &d){
    // qDebug()<<"add layer";
    // qDebug()<<QString::fromStdString(d.name);
}
void DxfLib::addXDataString(int, const std::string &){
    //qDebug()<<"add xdata string";
}
void DxfLib::addXRecord(const std::string &){
    // qDebug()<<"add repcord";
}
void DxfLib::addTrace(const DL_TraceData &){
    //  qDebug()<<"add trace";
}
void DxfLib::endEntity(){
    // qDebug()<<"end entity";
    // qDebug()<<"attribute"<<this->getAttributes().getColor();
}
void DxfLib::endSequence(){
    //  qDebug()<<"end sequence ";
}
void DxfLib::setAttributes(const DL_Attributes &attrib){
    // qDebug()<<"set attribute";
}
void DxfLib::addHatch(const DL_HatchData &d){
    hatches.append(d);
    // qDebug()<<"add hatch"<< d.originX << d.originY << d.numLoops;
}
void DxfLib::addHatchEdge(const DL_HatchEdgeData &d){
    //  qDebug()<<"add hat edge data";
    hatchedges.append(d);
    if(this->getAttributes().getColor()==1){
        QString color="#ef3312";
        hatchcolors.append(color);
    }
    else if(this->getAttributes().getColor()==2){
        QString color="#efef33";
        hatchcolors.append(color);
    }
    else if(this->getAttributes().getColor()==3){
        QString color="#12ef33";
        hatchcolors.append(color);
    }
    else {
        QString color="#efefef";
        hatchcolors.append(color);
    }
}
