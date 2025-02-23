// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef X11PLATFORMCLIPBOARD_H
#define X11PLATFORMCLIPBOARD_H

#include "platform/dummy/dummyclipboard.h"

#include <QByteArray>
#include <QStringList>
#include <QTimer>

#include <memory>

class X11PlatformClipboard final : public DummyClipboard
{
public:
    X11PlatformClipboard();

    void startMonitoring(const QStringList &formats) override;

    void setMonitoringEnabled(ClipboardMode mode, bool enable) override;

    QVariantMap data(ClipboardMode mode, const QStringList &formats) const override;

    void setData(ClipboardMode mode, const QVariantMap &dataMap) override;

    const QMimeData *mimeData(ClipboardMode mode) const override;

    bool isSelectionSupported() const override { return m_selectionSupported; }

protected:
    void onChanged(int mode) override;

private:
    struct ClipboardData {
        QVariantMap newData;
        QVariantMap data;
        QByteArray owner;
        QByteArray newOwner;
        QTimer timerEmitChange;
        QStringList formats;
        quint32 newDataTimestamp;
        ClipboardMode mode;
        bool enabled = true;
        bool cloningData = false;
        bool abortCloning = false;
        int retry = 0;
    };

    void check();
    void updateClipboardData(ClipboardData *clipboardData);
    void useNewClipboardData(ClipboardData *clipboardData);
    void checkAgainLater(bool clipboardChanged, int interval);

    QTimer m_timerCheckAgain;

    ClipboardData m_clipboardData;
    ClipboardData m_selectionData;

    bool m_monitoring = false;
    bool m_selectionSupported = true;
};

#endif // X11PLATFORMCLIPBOARD_H
