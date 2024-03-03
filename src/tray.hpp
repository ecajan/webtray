#ifndef TRAY_HPP
#define TRAY_HPP

#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>

#include "webwindow.hpp"

class Tray : public QSystemTrayIcon
{
private:
	QMenu menu;
	QMenu settings;
	QAction *app_toggle;
	QAction *quit;

	struct
	{
		QAction *camera;
		QAction *microphone;
		QAction *screenshare;
		QAction *notifications;
		QAction *location;
		QAction *lock_mouse;
	} permissions;

	QAction *reset_storage;
	std::function<void()> toggle_signal;
	std::function<void()> quit_signal;
	std::function<void()> reset_cookies_signal;
	std::function<void(QWebEnginePage::Feature, bool)> permission_changed_signal;

public:
	Tray();
	void connect_toggle(std::function<void()> fn);
	void connect_quit(std::function<void()> fn);
	void connect_reset_cookies(std::function<void()> fn);
	void connect_permission_changed(
		std::function<void(QWebEnginePage::Feature, bool)> fn);
	void set_title(const QString &title);
	void send_notification(std::unique_ptr<QWebEngineNotification> notification);
	void set_permission(QWebEnginePage::Feature feature, bool value);
};

#endif
