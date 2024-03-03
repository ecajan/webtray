#include "permissionmanager.hpp"

PermissionManager::PermissionManager(std::string path)
	: path(path)
{
	if (std::filesystem::exists(this->path)) {
		std::ifstream in(path);
		std::string content;
		in >> this->_lock_mouse;
		in >> this->_location;
		in >> this->_notification;
		in >> this->_media_audio_capture;
		in >> this->_media_video_capture;
		in >> this->_desktop_video_capture;
		in >> this->_desktop_audio_video_capture;
	}
}

PermissionManager::~PermissionManager()
{
	this->save();
}

void
PermissionManager::save()
{
	std::ofstream out(this->path);
	out << this->_lock_mouse;
	out << this->_location;
	out << this->_notification;
	out << this->_media_audio_capture;
	out << this->_media_video_capture;
	out << this->_desktop_video_capture;
	out << this->_desktop_audio_video_capture;
}

bool
PermissionManager::get(QWebEnginePage::Feature feature)
{
	switch (feature) {
		case QWebEnginePage::Feature::MouseLock:
			return this->_lock_mouse;
		case QWebEnginePage::Feature::Geolocation:
			return this->_location;
		case QWebEnginePage::Feature::Notifications:
			return this->_notification;
		case QWebEnginePage::Feature::MediaAudioCapture:
			return this->_media_audio_capture;
		case QWebEnginePage::Feature::MediaVideoCapture:
			return this->_media_video_capture;
		case QWebEnginePage::Feature::MediaAudioVideoCapture:
			return this->_media_video_capture && this->_media_audio_capture;
		case QWebEnginePage::Feature::DesktopVideoCapture:
			return this->_desktop_video_capture;
		case QWebEnginePage::Feature::DesktopAudioVideoCapture:
			return this->_desktop_audio_video_capture;
		default:
			/* unreachable except QWebEnginePage::Feature gets new entries */
			return false;
	}
}

void
PermissionManager::set(QWebEnginePage::Feature feature, bool value)
{
	switch (feature) {
		case QWebEnginePage::Feature::MouseLock:
			this->_lock_mouse = value;
			break;
		case QWebEnginePage::Feature::Geolocation:
			this->_location = value;
			break;
		case QWebEnginePage::Feature::Notifications:
			this->_notification = value;
			break;
		case QWebEnginePage::Feature::MediaAudioCapture:
			this->_media_audio_capture = value;
			break;
		case QWebEnginePage::Feature::MediaVideoCapture:
			this->_media_video_capture = value;
			break;
		case QWebEnginePage::Feature::MediaAudioVideoCapture:
			this->_media_audio_capture = value;
			this->_media_video_capture = value;
			break;
		case QWebEnginePage::Feature::DesktopVideoCapture:
			this->_desktop_video_capture = value;
			break;
		case QWebEnginePage::Feature::DesktopAudioVideoCapture:
			this->_desktop_audio_video_capture = value;
			break;
	}
}
