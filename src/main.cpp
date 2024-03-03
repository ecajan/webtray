#include <QtGui/QCloseEvent>
#include <QtWebEngineCore/QWebEngineNotification>
#include <QtWebEngineCore/QWebEnginePage>
#include <QtWebEngineCore/QWebEngineProfile>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QWidget>
#include <iostream>

class MainWindow : public QMainWindow
{
public:
	void closeEvent(QCloseEvent *event)
	{
		this->hide();
		event->ignore();
	}
};

int
main(int argc, char **argv)
{
	QApplication app(argc, argv);
	bool start_hidden = false;
	QUrl url;

	for (auto argument : app.arguments()) {
		if (argument == "--hidden") {
			start_hidden = true;
		} else {
			url = argument;
		}
	}

	if (url.url().toStdString() == argv[0]) {
		std::cerr << "webtray <url> [--hidden]\n";
		return -1;
	}

	MainWindow main_window;

	QSystemTrayIcon tray;
	QMenu menu;

	QWebEngineProfile profile(url.host().toStdString().c_str());
	QWebEnginePage page(&profile);
	QWebEngineView view;

	QAction *app_action = menu.addAction("Element");
	menu.addSeparator();
	QAction *quit_action = menu.addAction("Quit");

	menu.connect(&menu, &QMenu::triggered, [&](QAction *action) {
		if (action == app_action) {
			main_window.setVisible(!main_window.isVisible());
		} else if (action == quit_action) {
			main_window.close();
			page.windowCloseRequested();
			app.quit();
		}
	});

	view.setPage(&page);
	view.setUrl(url);

	tray.setContextMenu(&menu);

	tray.connect(&tray,
	             &QSystemTrayIcon::activated,
	             [&](const QSystemTrayIcon::ActivationReason reason) {
								 switch (reason) {
									 case QSystemTrayIcon::Trigger:
										 main_window.setVisible(!main_window.isVisible());
										 break;
									 default:
										 break;
								 }
							 });

	view.connect(&view, &QWebEngineView::iconChanged, [&](const QIcon icon) {
		tray.setIcon(icon);
		tray.show();
	});

	view.connect(&view, &QWebEngineView::titleChanged, [&](const QString title) {
		tray.setToolTip(title);
		app_action->setText(title);
	});

	page.settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
	page.settings()->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly,
	                              false);
	page.settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,
	                              false);

	profile.setPushServiceEnabled(true);
	profile.setNotificationPresenter(
		[&](std::unique_ptr<QWebEngineNotification> notification) {
			tray.showMessage(notification->title(),
		                   notification->message(),
		                   QSystemTrayIcon::MessageIcon::Information,
		                   3000);
		});

	page.connect(&page,
	             &QWebEnginePage::featurePermissionRequested,
	             [&](const QUrl origin, QWebEnginePage::Feature feature) {
								 QString feature_name;

								 switch (feature) {
									 case QWebEnginePage::Feature::MouseLock:
										 feature_name = "lock the mouse";
										 break;
									 case QWebEnginePage::Feature::Geolocation:
										 feature_name = "your location information";
										 break;
									 case QWebEnginePage::Feature::Notifications:
										 feature_name = "send notifications";
										 break;
									 case QWebEnginePage::Feature::MediaAudioCapture:
										 feature_name = "capture audio";
										 break;
									 case QWebEnginePage::Feature::MediaVideoCapture:
										 feature_name = "capture video";
										 break;
									 case QWebEnginePage::Feature::MediaAudioVideoCapture:
										 feature_name = "capture audio and video";
										 break;
									 case QWebEnginePage::Feature::DesktopVideoCapture:
										 feature_name = "capture video from your desktop";
										 break;
									 case QWebEnginePage::Feature::DesktopAudioVideoCapture:
										 feature_name = "capture audio and video from your desktop";
										 break;
								 }

								 QMessageBox dialog(QMessageBox::Icon::Question,
		                                "Permission",
		                                "Do you want to grant permission to " +
		                                  feature_name + "?",
		                                QMessageBox::StandardButton::Yes |
		                                  QMessageBox::StandardButton::No);
								 int res = dialog.exec();

								 if (res == QMessageBox::Yes) {
									 page.setFeaturePermission(
										 origin, feature, QWebEnginePage::PermissionGrantedByUser);
								 } else {
									 page.setFeaturePermission(
										 origin, feature, QWebEnginePage::PermissionDeniedByUser);
								 }
							 });

	main_window.setCentralWidget(&view);
	if (!start_hidden) {
		main_window.show();
	} else {
		tray.show();
	}
	return app.exec();
}
