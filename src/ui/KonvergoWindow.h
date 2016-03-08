#ifndef KONVERGOWINDOW_H
#define KONVERGOWINDOW_H

#include <QQuickWindow>
#include <QEvent>

///////////////////////////////////////////////////////////////////////////////////////////////////
class KonvergoWindow : public QQuickWindow
{
  Q_OBJECT

  Q_PROPERTY(bool fullScreen READ isFullScreen WRITE setFullScreen NOTIFY fullScreenSwitched)
  Q_PROPERTY(bool showDebugLayer MEMBER m_debugLayer NOTIFY debugLayerChanged)
  Q_PROPERTY(QString debugInfo MEMBER m_debugInfo NOTIFY debugInfoChanged)
  Q_PROPERTY(QString videoInfo MEMBER m_videoInfo NOTIFY debugInfoChanged)
  Q_PROPERTY(qreal webScale READ webScale NOTIFY webScaleChanged)

public:
  static void RegisterClass();

  KonvergoWindow(QWindow* parent = 0);
  ~KonvergoWindow();

  bool isFullScreen()
  {
    return ((flags() & Qt::FramelessWindowHint) || (visibility() == QWindow::FullScreen));
  }

  void setFullScreen(bool enable);

  Q_SLOT void otherAppFocus()
  {
    setWindowState((Qt::WindowState)((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive));
    raise();
  }

  Q_SLOT void toggleDebug();

  Q_SLOT void toggleFullscreen()
  {
    setFullScreen(!isFullScreen());
  }

  Q_SLOT void reloadWeb()
  {
    emit reloadWebClient();
  }

  qreal webScale() { return CalculateScale(size()); }
  static qreal CalculateScale(const QSize& size);

Q_SIGNALS:
  void fullScreenSwitched();
  void webScaleChanged();
  void enableVideoWindowSignal();
  void debugLayerChanged();
  void debugInfoChanged();
  void reloadWebClient();

protected:
  virtual void focusOutEvent(QFocusEvent* ev) override;
  virtual void resizeEvent(QResizeEvent* event) override;

private slots:
  void closingWindow();
  void enableVideoWindow();
  void onVisibilityChanged(QWindow::Visibility visibility);
  void updateMainSectionSettings(const QVariantMap& values);
  void updateFullscreenState();
  void onScreenCountChanged(int newCount);
  void updateDebugInfo();
  void playerWindowVisible(bool visible);
  void playerPlaybackStarting();

private:
  void saveGeometry();
  void loadGeometry();
  QRect loadGeometryRect();
  bool m_debugLayer;
  qreal m_lastScale;
  QTimer* m_infoTimer;
  QString m_debugInfo, m_systemDebugInfo, m_videoInfo;
};

#endif // KONVERGOWINDOW_H
