#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "permissionmanager.hpp"
#include <QtCore/QUrl>
#include <QtWebEngineCore/QWebEngineNotification>
#include <QtWebEngineCore/QWebEnginePage>
#include <QtWebEngineCore/QWebEngineProfile>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QMainWindow>
#include <functional>

class WebWindow : public QMainWindow
{
private:
	QWebEngineProfile _profile;
	QWebEnginePage _page;
	QWebEngineView _view;
	PermissionManager _permissions;

	void web_configure();
	void permission_requested(const QUrl origin, QWebEnginePage::Feature feature);
	void closeEvent(QCloseEvent *event);

public:
	WebWindow(const QString &url);
	void connect_icon_changed(std::function<void(const QIcon)> fn);
	void connect_title_changed(std::function<void(const QString)> fn);
	void connect_notification(std::function<void(std::unique_ptr<QWebEngineNotification>)> fn);
	void set_feature(QWebEnginePage::Feature feature, bool value);
	void reset_cookies();
	void toggle_visibility();
	void quit();
};

#endif
