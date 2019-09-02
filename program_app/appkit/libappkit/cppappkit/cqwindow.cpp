#include "cqwindow.hh"
#include "cqapplication.hh"
#include "cqgraphics.hh"
#include "cqtouchesevent_p.hh"
#include "cqwindow.h"

cq_member(cqWindow) {
    cq_window *window = nullptr;
    
    cqViewControllerRef rootViewController;
    cqViewRef touchesResponder;
};

void cqWindow::setRootViewController(cqViewControllerRef controller) {
    if (dat->rootViewController == controller) {
        return;
    }
    
    if (dat->rootViewController != nullptr) {
        dat->rootViewController->view()->removeFromSuperview();
    }
    dat->rootViewController = controller;
    addSubview(dat->rootViewController->view());
}

cqViewControllerRef cqWindow::rootViewController() {
    return dat->rootViewController;
}

static void load(cq_window *window) {
    auto self = (cqWindow *)cq_window_extra(window);
    
    cqRect rect; {
        rect.size.width  = cq_window_width (window);
        rect.size.height = cq_window_height(window);
    }
    self->setFrame(rect);
    
    cqGraphics::startupGraphicsProgram(self->strongRef());
}

static void appear(cq_window *window) {
    //auto self = (cqWindow *)cq_window_extra(window);
    
    auto delegate = cqApplication::get()->delegate();
    if (delegate != nullptr) {
        delegate->applicationWillEnterForeground();
    }
}

static void disappear(cq_window *window) {
    //auto self = (cqWindow *)cq_window_extra(window);
    
    auto delegate = cqApplication::get()->delegate();
    if (delegate != nullptr) {
        delegate->applicationDidEnterBackground();
    }
}

static void resize(cq_window *window, float width, float height) {
    auto self = (cqWindow *)cq_window_extra(window);
    
    self->setFrame(cqRect(0, 0, width, height));
}

static void gldraw(cq_window *window) {
    auto self = (cqWindow *)cq_window_extra(window);
    
    cqGraphics::prepareDraw();
    
    cqGraphics::pushContext(cqContext(0, 0));
    self->drawSelfAndSubviews();
    cqGraphics::popContext();
}

static void pbegan(cq_window *window, float x, float y) {
    auto self = (cqWindow *)cq_window_extra(window);
    
    std::set<cqTouchRef> touches = {
        cqTouch::createWithLocation(self->strongRef(), cqPoint(x, y))
    };
    cqTouchesEventRef touchesEvent = cqTouchesEvent::create();
    
    //hit test
    auto view = self->hitTest(cqPoint(x, y), touchesEvent);
    if (view == nullptr) {
        return;
    }
    
    //began event
    view->touchesBegan(touches, touchesEvent);
    self->dat->touchesResponder = view;
}

static void pmoved(cq_window *window, float x, float y) {
    auto self = (cqWindow *)cq_window_extra(window);
    if (self->dat->touchesResponder == nullptr) {
        return;
    }
    
    std::set<cqTouchRef> touches = {
        cqTouch::createWithLocation(self->strongRef(), cqPoint(x, y))
    };
    cqTouchesEventRef touchesEvent = cqTouchesEvent::create();
    
    self->dat->touchesResponder->touchesMoved(touches, touchesEvent);
}

static void pended(cq_window *window, float x, float y) {
    auto self = (cqWindow *)cq_window_extra(window);
    if (self->dat->touchesResponder == nullptr) {
        return;
    }
    
    std::set<cqTouchRef> touches = {
        cqTouch::createWithLocation(self->strongRef(), cqPoint(x, y))
    };
    cqTouchesEventRef touchesEvent = cqTouchesEvent::create();
    
    self->dat->touchesResponder->touchesEnded(touches, touchesEvent);
    self->dat->touchesResponder.reset();
}

void cqWindow::makeKeyAndVisible() {
    
    cq_procedure procedure = {nullptr};
    procedure.load = load;
    procedure.appear = appear;
    procedure.disappear = disappear;
    procedure.resize = resize;
    procedure.gldraw = gldraw;
    procedure.pbegan = pbegan;
    procedure.pmoved = pmoved;
    procedure.pended = pended;
    
    dat->window = cq_create_window();
    cq_set_procedure(dat->window, &procedure);
    cq_set_window_extra(dat->window, (int64_t)this);
    cq_show_window(dat->window);
}

cqResponderRef cqWindow::nextResponder() {
    return cqApplication::get();
}

void cqWindow::setFrame(cqRect frame) {
    super::setFrame(frame);
    if (dat->rootViewController != nullptr) {
        dat->rootViewController->view()->setFrame(frame);
    }
}