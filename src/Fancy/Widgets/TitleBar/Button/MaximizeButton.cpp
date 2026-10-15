//
// Created by TX on 2025/9/19.
//

#include "MaximizeButton.h"

#include "Core/Defs.h"
#include "Core/Palette/Palette.h"
#include "Widgets/SvgWidget/SvgWidget.h"

namespace fancy
{
    MaximizeButton::MaximizeButton(QWidget *parent) :
        UniversalTitleBarButton(parent)
    {
        svgWidget()->reload(AntDesignIcons::Square);

        connect(&Palette::palette(), &Palette::appThemeChange, svgWidget(), &SvgWidget::load);
        connect(this, &MaximizeButton::enter, this, &MaximizeButton::onEnter);
        connect(this, &MaximizeButton::leave, this, &MaximizeButton::onLeave);
        connect(this, &MaximizeButton::mousePress, this, &MaximizeButton::onMousePress);
        connect(this, &MaximizeButton::mouseRelease, this, &MaximizeButton::onMouseRelease);
        connect(this, &MaximizeButton::mouseMove, this, &MaximizeButton::onMouseMove);
        connect(this, &MaximizeButton::stateChange, this, &MaximizeButton::loadIcon);

        // 窗口摘除 WS_POPUP 后（Window::showEvent），HTMAXBUTTON 命中区的
        // 鼠标交互被系统接管，nativeEvent 的 NCLBUTTONUP 路径失效，点击
        // 由按钮自身处理。不用 showMaximized/showNormal：Qt 6.8+ 存在窗口
        // 动画消失、大小无法复原的 bug，SC_* 消息无此问题。
        connect(this, &MaximizeButton::clicked, this, [this]() {
            if (!window())
                return;
            const HWND hwnd = reinterpret_cast<HWND>(window()->winId());
            if (window()->windowState() & Qt::WindowMaximized)
                ::SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            else
                ::SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        });
    }


    void MaximizeButton::loadIcon(MaximizeBtnState state) const
    {
        if (state == MaximizeBtnState::Normal)
            svgWidget()->reload(AntDesignIcons::Square);
        else
            svgWidget()->reload(AntDesignIcons::SquareMultiple);
    }

    void MaximizeButton::onEnter()
    {
        setTitleBtnState(TitleBtnState::Hover);
        update();
    }

    void MaximizeButton::onLeave()
    {
        setTitleBtnState(TitleBtnState::Normal);
        update();
    }

    void MaximizeButton::onMousePress()
    {
        setTitleBtnState(TitleBtnState::Pressed);
        update();
    }

    void MaximizeButton::onMouseRelease(bool isInSelf)
    {
        if (isInSelf)
            setTitleBtnState(TitleBtnState::Hover);
        else
            setTitleBtnState(TitleBtnState::Normal);
        update();
    }

    void MaximizeButton::onMouseMove(bool isInSelf)
    {
        if (isInSelf)
            setTitleBtnState(TitleBtnState::Pressed);
        else
            setTitleBtnState(TitleBtnState::PressedOut);
        update();
    }
} // fancy
