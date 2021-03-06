#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glstdafx.h"

#include <QVector3D>
#include <QMatrix4x4>

YGL_BEGIN_NAMESPACE

class Transform
{
public:
    Transform();
    ~Transform();

public:
    // operators
    void translate ( const QVector3D &offset );
    void translate ( float x, float y, float z );
    void scale ( float x, float y, float z );
    void rotate ( float angle, const QVector3D &axis );
    void setPosition( const QVector3D &pos );
    void lookAt(const QVector3D &eye, const QVector3D &target , const QVector3D &up);

    // getter
    const QMatrix4x4 &getObjectToWorldMatrix() const;
    const QMatrix4x4 &getWorldToObjectMatrix() const;
    QVector3D getPosition() const;

private:
    QMatrix4x4 m_objectToWorldMatrix;
    QMatrix4x4 m_worldToObjectMatrix;
};

YGL_END_NAMESPACE

#endif // TRANSFORM_H
