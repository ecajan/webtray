#include "tray.hpp"

Tray::Tray()
{
	this->app_toggle = this->menu.addAction("");
	this->menu.addSeparator();
	this->menu.addMenu(&this->settings)->setText("Settings");
	this->menu.addSeparator();
	this->quit = this->menu.addAction("Quit");

	this->permissions.camera = this->settings.addAction("Allow Camera");
	this->permissions.microphone = this->settings.addAction("Allow Microphone");
	this->permissions.screenshare = this->settings.addAction("Allow Screenshare");
	this->permissions.notifications =
		this->settings.addAction("Allow Notifications");
	this->permissions.location = this->settings.addAction("Allow Location");
	this->permissions.lock_mouse = this->settings.addAction("Allow Lock Mouse");
	this->settings.addSeparator();
	this->reset_storage = this->settings.addAction("Reset Storage");

	this->permissions.camera->setCheckable(true);
	this->permissions.microphone->setCheckable(true);
	this->permissions.screenshare->setCheckable(true);
	this->permissions.notifications->setCheckable(true);
	this->permissions.location->setCheckable(true);
	this->permissions.lock_mouse->setCheckable(true);

	this->connect(this,
	              &QSystemTrayIcon::activated,
	              [&](QSystemTrayIcon::ActivationReason reason) {
									if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
										this->toggle_signal();
									}
								});

	this->menu.connect(&this->menu, &QMenu::triggered, [&](QAction *action) {
		if (action == this->app_toggle) {
			this->toggle_signal();
		} else if (action == quit) {
			this->quit_signal();
		}
	});

	this->settings.connect(
		&this->settings, &QMenu::triggered, [&](QAction *action) {
			if (action == this->reset_storage) {
				this->reset_cookies_signal();

			} else if (action == this->permissions.camera) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::MediaVideoCapture,
					this->permissions.camera->isChecked());

			} else if (action == this->permissions.microphone) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::MediaAudioCapture,
					this->permissions.microphone->isChecked());

			} else if (action == this->permissions.screenshare) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::DesktopAudioVideoCapture,
					this->permissions.screenshare->isChecked());

			} else if (action == this->permissions.notifications) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::Notifications,
					this->permissions.notifications->isChecked());

			} else if (action == this->permissions.location) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::Geolocation,
					this->permissions.location->isChecked());

			} else if (action == this->permissions.lock_mouse) {
				this->permission_changed_signal(
					QWebEnginePage::Feature::MouseLock,
					this->permissions.lock_mouse->isChecked());
			}
		});

	this->setContextMenu(&this->menu);
}

void
Tray::connect_toggle(std::function<void()> fn)
{
	this->toggle_signal = fn;
}

void
Tray::connect_quit(std::function<void()> fn)
{
	this->quit_signal = fn;
}

void
Tray::connect_reset_cookies(std::function<void()> fn)
{
	this->reset_cookies_signal = fn;
}

void
Tray::connect_permission_changed(
	std::function<void(QWebEnginePage::Feature, bool)> fn)
{
	this->permission_changed_signal = fn;
}

void
Tray::set_title(const QString &title)
{
	this->setToolTip(title);
	this->app_toggle->setText(title);
}

void
Tray::send_notification(std::unique_ptr<QWebEngineNotification> notification)
{
	this->showMessage(notification->title(),
	                  notification->message(),
	                  QSystemTrayIcon::MessageIcon::Information,
	                  2000);
}

void
Tray::set_permission(QWebEnginePage::Feature feature, bool value)
{
	switch (feature) {
		case QWebEnginePage::Feature::MouseLock:
			this->permissions.lock_mouse->setChecked(value);
			break;
		case QWebEnginePage::Feature::Geolocation:
			this->permissions.location->setChecked(value);
			break;
		case QWebEnginePage::Feature::Notifications:
			this->permissions.notifications->setChecked(value);
			break;
		case QWebEnginePage::Feature::MediaAudioCapture:
			this->permissions.microphone->setChecked(value);
			break;
		case QWebEnginePage::Feature::MediaVideoCapture:
			this->permissions.camera->setChecked(value);
			break;
		case QWebEnginePage::Feature::MediaAudioVideoCapture:
			this->permissions.microphone->setChecked(value);
			this->permissions.camera->setChecked(value);
			break;
		case QWebEnginePage::Feature::DesktopVideoCapture:
			this->permissions.screenshare->setChecked(value);
			break;
		case QWebEnginePage::Feature::DesktopAudioVideoCapture:
			this->permissions.screenshare->setChecked(value);
			break;
	}
}
