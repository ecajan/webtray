#include <QtGui/QCloseEvent>
#include <QtGui/QDesktopServices>
#include <QtWebEngineCore/QWebEngineCookieStore>
#include <QtWebEngineCore/QWebEngineNewWindowRequest>
#include <QtWidgets/QApplication>
#include <iostream>

#include "permissionmanager.hpp"
#include "webwindow.hpp"

WebWindow::WebWindow(const QString &url)
	: QMainWindow()
	, profile(QUrl(url).host())
	, page(&profile)
	, perm((profile.persistentStoragePath() + "/permissions.state").toStdString())
{
	this->web_configure();

	this->profile.setPersistentCookiesPolicy(
		QWebEngineProfile::ForcePersistentCookies);

	this->view.setPage(&this->page);
	this->view.setUrl(url);

	this->setCentralWidget(&this->view);
}

void
WebWindow::web_configure()
{
	this->page.settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled,
	                                    true);

	this->page.settings()->setAttribute(
		QWebEngineSettings::WebRTCPublicInterfacesOnly, false);

	this->page.settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,
	                                    false);

	this->profile.setPushServiceEnabled(true);

	this->page.connect(&this->page,
	                   &QWebEnginePage::featurePermissionRequested,
	                   [&](const QUrl origin, QWebEnginePage::Feature feature) {
											 this->permission_requested(origin, feature);
										 });

	this->page.connect(&this->page,
	                   &QWebEnginePage::newWindowRequested,
	                   [=](QWebEngineNewWindowRequest &request) {
											 QDesktopServices::openUrl(request.requestedUrl());
										 });
}

void
WebWindow::permission_requested(const QUrl origin,
                                QWebEnginePage::Feature feature)
{
	if (this->perm.get(feature)) {
		this->page.setFeaturePermission(
			origin, feature, QWebEnginePage::PermissionGrantedByUser);
	} else {
		this->page.setFeaturePermission(
			origin, feature, QWebEnginePage::PermissionDeniedByUser);
	}
	this->page.setFeaturePermission(
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
	this->view.connect(&this->view, &QWebEngineView::iconChanged, fn);
}

void
WebWindow::connect_notification(
	std::function<void(std::unique_ptr<QWebEngineNotification>)> fn)
{
	this->profile.setNotificationPresenter(fn);
}

void
WebWindow::connect_title_changed(std::function<void(const QString)> fn)
{
	this->view.connect(&this->view, &QWebEngineView::titleChanged, fn);
}

PermissionManager &
WebWindow::permissions()
{
	return this->perm;
}

void
WebWindow::reset_cookies()
{
	this->profile.cookieStore()->deleteAllCookies();
	this->profile.cookieStore()->loadAllCookies();
	this->profile.clearHttpCache();
	this->profile.clearAllVisitedLinks();
	this->profile.clientCertificateStore();
	this->view.reload();
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
