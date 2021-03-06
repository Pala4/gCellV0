#include "cportal.h"

#include <QPainter>
#include <QAction>
#include <QSignalMapper>

#include "../link/clink.h"
#include "../DataBuffer/cdatabuffer.h"

QPainterPath CPortal::shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath())
        return path;

    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0) {
        ps.setWidth(penWidthZero);
    } else {
        ps.setWidth(pen.widthF());
    }
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);

    return p;
}

void CPortal::updateLinks()
{
    foreach (CLink *link, m_links) {
        if (link != nullptr)
            link->updateGeometry();
    }
}

QVariant CPortal::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemScenePositionHasChanged:
        updateLinks();
        break;
    default:
        break;
    }

    return CElement::itemChange(change, value);
}

QRectF CPortal::calcBounds()
{
    return shape().controlPointRect();
}

QPointF CPortal::captionEditorPosition()
{
    if (captionEditor() == nullptr)
        return CElement::captionEditorPosition();

    QRectF boundRect = shape().controlPointRect();
    QRectF captionBound = captionEditor()->boundingRect();
    QPointF captionPosition = captionEditor()->pos();
    switch (portalOrientation()) {
    case CPortal::Left:
        captionPosition.setX(-captionBound.width());
        break;
    case CPortal::Right:
        captionPosition.setX(boundRect.width());
        break;
    case CPortal::Top:
        captionPosition.setY(-captionBound.height());
        break;
    case CPortal::Bottom:
        captionPosition.setY(boundRect.height());
        break;
    default:
        break;
    }
    qreal minHeight = qMin(boundRect.height(), captionBound.height());
    qreal maxHeight = qMax(boundRect.height(), captionBound.height());
    int yOffsetSign = boundRect.height() < captionBound.height() ? -1 : 1;
    qreal minWidth = qMin(boundRect.width(), captionBound.width());
    qreal maxWidth = qMax(boundRect.width(), captionBound.width());
    int xOffsetSign = boundRect.width() < captionBound.width() ? -1 : 1;
    switch (portalOrientation()) {
    case CPortal::Left:
    case CPortal::Right:
        captionPosition.setY(yOffsetSign*(maxHeight - minHeight)/2.0);
        break;
    case CPortal::Top:
    case CPortal::Bottom:
        captionPosition.setX(xOffsetSign*(maxWidth - minWidth)/2.0);
        break;
    default:
        break;
    }

    return captionPosition;
}

CPortal::CPortal(QGraphicsItem *parent) : CElement(parent)
{
    setObjectName(QStringLiteral("CPortal"));

    m_portalOrientation = CPortal::NoOrientation;
    m_dataColor = QColor(255, 255, 255);
    m_size = 7.0;
    m_highLighted = false;
    m_checked = false;
    m_loopBackPortal = false;
    m_dataBuffer = nullptr;
    m_dataBufferIsReference = false;
    m_orientActionsMapper = nullptr;
    m_acLeftOrient = nullptr;
    m_acTopOrient = nullptr;
    m_acRightOrient = nullptr;
    m_acBottomOrient = nullptr;

    m_orientActionsMapper = new QSignalMapper(this);
    m_orientActionsMapper->setObjectName(QStringLiteral("orientActionsMapper"));
    connect(m_orientActionsMapper, SIGNAL(mapped(int)), this, SLOT(setPortalOrientation(int)));

    QActionGroup *acgrOrient = new QActionGroup(this);
    acgrOrient->setObjectName(QStringLiteral("acgrOrient"));

    m_acLeftOrient = new QAction(tr("Left"), this);
    m_acLeftOrient->setObjectName(QStringLiteral("acLeftOrient"));
    m_acLeftOrient->setCheckable(true);
    m_acLeftOrient->setChecked(true);
    connect(m_acLeftOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
    m_orientActionsMapper->setMapping(m_acLeftOrient, (int)CPortal::Left);
    addAction(m_acLeftOrient);
    acgrOrient->addAction(m_acLeftOrient);

    m_acTopOrient = new QAction(tr("Top"), this);
    m_acTopOrient->setObjectName(QStringLiteral("acTopOrient"));
    m_acTopOrient->setCheckable(true);
    connect(m_acTopOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
    m_orientActionsMapper->setMapping(m_acTopOrient, (int)CPortal::Top);
    addAction(m_acTopOrient);
    acgrOrient->addAction(m_acTopOrient);

    m_acRightOrient = new QAction(tr("Right"), this);
    m_acRightOrient->setObjectName(QStringLiteral("acRightOrient"));
    m_acRightOrient->setCheckable(true);
    connect(m_acRightOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
    m_orientActionsMapper->setMapping(m_acRightOrient, (int)CPortal::Right);
    addAction(m_acRightOrient);
    acgrOrient->addAction(m_acRightOrient);

    m_acBottomOrient = new QAction(tr("Bottom"), this);
    m_acBottomOrient->setObjectName(QStringLiteral("acBottomOrient"));
    m_acBottomOrient->setCheckable(true);
    connect(m_acBottomOrient, SIGNAL(triggered()), m_orientActionsMapper, SLOT(map()));
    m_orientActionsMapper->setMapping(m_acBottomOrient, (int)CPortal::Bottom);
    addAction(m_acBottomOrient);
    acgrOrient->addAction(m_acBottomOrient);

    setCaptionFont(QFont("Corier", 7, QFont::Bold));
    setInteractions(intercations() & ~CElement::Copyable & ~CElement::Deletable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

QString CPortal::caption()
{
    return captionPrefix() + CElement::caption() + captionPostfix();
}

void CPortal::setCaptionPrefix(const QString &captionPrefix)
{
    if (m_captionPrefix == captionPrefix)
        return;

    m_captionPrefix = captionPrefix;
    emit nameChanged(name());
    updateGeometry();
}

void CPortal::setCaptionPostfix(const QString &captionPostfix)
{
    if (m_captionPostfix == captionPostfix)
        return;

    m_captionPostfix = captionPostfix;
    emit nameChanged(name());
}

QPainterPath CPortal::shape() const
{
    return shapeFromPath(m_portalForm, QPen());
}

void CPortal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    CElement::paint(painter, option, widget);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if (isHighLighted()) {
        brush.setColor(QColor(255, 0, 0));
    } else if (isChecked()) {
        brush.setColor(QColor(128, 0, 255, 190));
    } else if (isLoopBackPortal()) {
        brush.setColor(QColor(0, 0, 0, 255));
    } else {
        if (dataColor().isValid()) {
            brush.setColor(dataColor());
        } else {
            brush.setColor(QColor(255, 255, 255, 0));
        }
    }

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(0, 255, 0, 100));

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawPath(m_portalForm);
    painter->restore();
}

void CPortal::setPortalOrientation(const CPortal::TPortalOrientation &portalOrientation)
{
    m_portalOrientation = portalOrientation;
    switch(m_portalOrientation)
    {
    case CPortal::Left:
        if (m_acLeftOrient != nullptr)
            m_acLeftOrient->setChecked(true);
        break;
    case CPortal::Top:
        if (m_acTopOrient != nullptr)
            m_acTopOrient->setChecked(true);
        break;
    case CPortal::Right:
        if (m_acRightOrient != nullptr)
            m_acRightOrient->setChecked(true);
        break;
    case CPortal::Bottom:
        if (m_acBottomOrient != nullptr)
            m_acBottomOrient->setChecked(true);
        break;
    default:
        break;
    }

    updateGeometry();
}

void CPortal::setDataColor(const QColor &dataColor)
{
    if (m_dataColor == dataColor)
        return;

    m_dataColor = dataColor;
    update();

    emit dataColorChanged(m_dataColor);
}

void CPortal::setHighLighted(const bool &highLighted)
{
    if (m_highLighted == highLighted)
        return;

    m_highLighted = highLighted;
    update();
}

void CPortal::setChecked(const bool &cheked)
{
    if (m_checked == cheked)
        return;

    m_checked = cheked;
    update();
}

void CPortal::setLoopBackPortal(const bool &loopBackPortal)
{
    if (m_loopBackPortal == loopBackPortal)
        return;

    m_loopBackPortal = loopBackPortal;
    update();
}

void CPortal::addLink(CLink *link)
{
    if ((link == nullptr) || (m_links.contains(link)))
        return;

    m_links << link;
}

void CPortal::removeLink(CLink *link)
{
    if ((link == nullptr) || !m_links.contains(link))
        return;

    m_links.removeOne(link);
}

CDataBuffer* CPortal::createBuffer()
{
    m_dataBufferIsReference = false;
    m_dataBuffer = new CDataBuffer(this);
    m_dataBuffer->setObjectName(QStringLiteral("dataBuffer"));
    return m_dataBuffer;
}

void CPortal::setBuffer(CDataBuffer *dataBuffer)
{
    if (!m_dataBufferIsReference && (m_dataBuffer != nullptr))
        return;

    m_dataBufferIsReference = true;
    m_dataBuffer = dataBuffer;
}

void CPortal::clearBuffer()
{
    if ((m_dataBuffer == nullptr) || m_dataBufferIsReference)
        return;

    m_dataBuffer->clear();
}

unsigned long long CPortal::bufferSize() const
{
    if (m_dataBuffer != nullptr)
        return m_dataBuffer->size();

    return 0ULL;
}

void CPortal::appendBuffer(const long double &ldblTimeFrame, const long double &ldblValue)
{
    if ((m_dataBuffer != nullptr) && !m_dataBufferIsReference)
        m_dataBuffer->append(ldblTimeFrame, ldblValue);
}

stData CPortal::bufferData(const unsigned long long &ullTFIndex)
{
    if (m_dataBuffer == nullptr)
        return stData();

    if (isLoopBackPortal()) {
        return m_dataBuffer->last();
    }

    return m_dataBuffer->data(ullTFIndex);
}

bool CPortal::isBufferDataExist(const unsigned long long &ullTFIndex)
{
    if (m_dataBuffer == nullptr)
        return false;

    return m_dataBuffer->contains(ullTFIndex);
}

void CPortal::beforeCalc(const long double &ldblStartTime, const long double &ldblTimeStep,
                         const long double &ldblEndTime)
{
    Q_UNUSED(ldblStartTime)
    Q_UNUSED(ldblTimeStep)
    Q_UNUSED(ldblEndTime)

    clearBuffer();
    setLoopBackPortal(false);
}

void CPortal::setPortalOrientation(const int &portalOrientation)
{
    setPortalOrientation((CPortal::TPortalOrientation)portalOrientation);
}

void CPortal::updateGeometry()
{
    prepareGeometryChange();
    m_portalForm = calcPortalForm();
    CElement::updateGeometry();
    emit geometryChanged();
    update();
}