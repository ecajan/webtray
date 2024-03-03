#include <QtGui/QCloseEvent>
#include <QtWebEngineCore/QWebEngineCookieStore>

#include "permissionmanager.hpp"
#include "webwindow.hpp"
#include <QtWidgets/QApplication>
#include <iostream>

WebWindow::WebWindow(const QString &url)
	: QMainWindow()
	, _profile(QUrl(url).host())
	, _page(&_profile)
	, _permissions((QUrl(url).host() + "/permissions.state").toStdString())
{
	this->web_configure();

	this->_profile.setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

	this->_view.setPage(&this->_page);
	this->_view.setUrl(url);

	this->setCentralWidget(&this->_view);
}

void
WebWindow::web_configure()
{
	this->_page.settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled,
	                                     true);

	this->_page.settings()->setAttribute(
		QWebEngineSettings::WebRTCPublicInterfacesOnly, false);

	this->_page.settings()->setAttribute(
		QWebEngineSettings::ScrollAnimatorEnabled, false);

	this->_profile.setPushServiceEnabled(true);

	this->_page.connect(&this->_page,
	                    &QWebEnginePage::featurePermissionRequested,
	                    [&](const QUrl origin, QWebEnginePage::Feature feature) {
												this->permission_requested(origin, feature);
											});
}

void
WebWindow::permission_requested(const QUrl origin,
                                QWebEnginePage::Feature feature)
{
	if (this->_permissions.get(feature)) {
		this->_page.setFeaturePermission(
			origin, feature, QWebEnginePage::PermissionGrantedByUser);
	} else {
		this->_page.setFeaturePermission(
			origin, feature, QWebEnginePage::PermissionDeniedByUser);
	}
	this->_page.setFeaturePermission(
		origin, feature, QWebEnginePage::PermissionUnknown);
}

void
WebWindow::closeEvent(QCloseEvent *event)
{
	this->hide();
	event->ignore();
}

void
WebWindow::connect_icon_changed(std::function<void(const QIcon)> fn)
{
	this->_view.connect(&this->_view, &QWebEngineView::iconChanged, fn);
}

void
WebWindow::connect_notification(
	std::function<void(std::unique_ptr<QWebEngineNotification>)> fn)
{
	this->_profile.setNotificationPresenter(fn);
}

void
WebWindow::connect_title_changed(std::function<void(const QString)> fn)
{
	this->_view.connect(&this->_view, &QWebEngineView::titleChanged, fn);
}

void
WebWindow::set_feature(QWebEnginePage::Feature feature, bool value)
{
	this->_permissions.set(feature, value);
}

void
WebWindow::reset_cookies()
{
	this->_profile.cookieStore()->deleteAllCookies();
	this->_profile.cookieStore()->loadAllCookies();
	this->_profile.clearHttpCache();
	this->_profile.clearAllVisitedLinks();
	this->_profile.clientCertificateStore();
	this->_view.reload();
}

void
WebWindow::toggle_visibility()
{
	this->setVisible(!this->isVisible());
}

void
WebWindow::quit()
{
	QApplication::instance()->quit();
}
