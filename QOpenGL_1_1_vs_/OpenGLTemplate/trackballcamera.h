#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

#include "glstdafx.h"

#include "camera.h"

#include "QVector2D"

YGL_BEGIN_NAMESPACE

/* 按照當前設定的角度(angle)與距離(distance)自動面向目標(target)*/

class TrackBallCamera : public Camera
{
public:
    TrackBallCamera();
    ~TrackBallCamera();

public:
    QVector3D  targetPosition;

public:
    void rotate ( QVector2D delta );
    void translate ( QVector2D delta );
    void zoom ( qreal delta );

protected:
    void update();

public:
    bool    enabled;

private:
    qreal   angleH; // degree !!
    qreal   angleV;
    qreal   maxAngleV;
    qreal   minAngleV;

    qreal   distance;
    qreal   minDistance;
};

YGL_END_NAMESPACE

#endif // TRACKBALLCAMERA_H
