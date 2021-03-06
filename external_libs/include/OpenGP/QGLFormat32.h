#pragma once
#include <QGLFormat>

//== NAMESPACE ================================================================
namespace opengp {
//=============================================================================

class QGLFormat32 : public QGLFormat {
public:
    QGLFormat32() {
        setVersion(3,2);
        setProfile(QGLFormat::CoreProfile);
        setSampleBuffers(true); ///< anti-aliasing
    }
};

//=============================================================================
} // namespace opengp
//=============================================================================
