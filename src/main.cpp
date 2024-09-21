#include <QtWidgets/QApplication>

#include <iostream>

using namespace std::chrono_literals;

#include "permissionmanager.hpp"
#include "tray.hpp"
#include "webwindow.hpp"

const QWebEnginePage::Feature features[] = {
	QWebEnginePage::Feature::MouseLock,
	QWebEnginePage::Feature::Notifications,
	QWebEnginePage::Feature::Notifications,
	QWebEnginePage::Feature::DesktopVideoCapture,
	QWebEnginePage::Feature::DesktopAudioVideoCapture,
	QWebEnginePage::Feature::MediaAudioCapture,
	QWebEnginePage::Feature::MediaVideoCapture,
};

QString
extract_url(const QStringList arguments)
{
	for (const auto &argument : arguments) {
		if (!argument.endsWith("webtray") && !argument.startsWith("--")) {
			return argument;
		}
	}
	return "";
}

int
main(int argc, char **argv)
{
	QApplication *app = new QApplication(argc, argv);
	app->setQuitOnLastWindowClosed(false);

	QString url = extract_url(app->arguments());

	if (url.isEmpty()) {
		std::cerr << "webtray [--open-at-startup] <url>\n";
		return -1;
	}

	WebWindow *webwindow = new WebWindow(url);
	Tray *tray = new Tray();

	bool start_hidden = not app->arguments().contains("--open-at-startup");

	for (auto feature : features) {
		tray->set_permission(feature, webwindow->permissions().get(feature));
	}

	webwindow->connect_icon_changed([&](auto icon) {
		tray->setIcon(icon);
		tray->show();
	});

	webwindow->connect_title_changed([&](auto title) { tray->set_title(title); });

	webwindow->connect_notification([&](auto notification) {
		tray->send_notification(std::move(notification));
	});

	tray->connect_toggle([&]() { webwindow->toggle_visibility(); });
	tray->connect_quit([&]() { webwindow->quit(); });
	tray->connect_permission_changed([&](auto feature, auto value) {
		webwindow->permissions().set(feature, value);
	});
	tray->connect_reset_cookies([&]() { webwindow->reset_cookies(); });

	webwindow->show();
	if (start_hidden) {
		webwindow->hide();
	}

	return app->exec();
}
