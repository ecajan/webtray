#include "permissionmanager.hpp"
#include <iostream>

PermissionManager::PermissionManager(std::string path)
	: path(path)
{
	if (std::filesystem::exists(this->path)) {
		std::ifstream in(path);
		std::string content;
		in >> this->lock_mouse;
		in >> this->location;
		in >> this->notification;
		in >> this->media_audio_capture;
		in >> this->media_video_capture;
		in >> this->desktop_audio_video_capture;
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
	out << this->lock_mouse;
	out << " ";
	out << this->location;
	out << " ";
	out << this->notification;
	out << " ";
	out << this->media_audio_capture;
	out << " ";
	out << this->media_video_capture;
	out << " ";
	out << this->desktop_audio_video_capture;
}

bool
PermissionManager::get(QWebEnginePage::Feature feature)
{
	switch (feature) {
		case QWebEnginePage::Feature::MouseLock:
			return this->lock_mouse;
		case QWebEnginePage::Feature::Geolocation:
			return this->location;
		case QWebEnginePage::Feature::Notifications:
			return this->notification;
		case QWebEnginePage::Feature::MediaAudioCapture:
			return this->media_audio_capture;
		case QWebEnginePage::Feature::MediaVideoCapture:
			return this->media_video_capture;
		case QWebEnginePage::Feature::MediaAudioVideoCapture:
			return this->media_video_capture && this->media_audio_capture;
		case QWebEnginePage::Feature::DesktopVideoCapture:
			return this->desktop_audio_video_capture;
		case QWebEnginePage::Feature::DesktopAudioVideoCapture:
			return this->desktop_audio_video_capture;
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
			this->lock_mouse = value;
			break;
		case QWebEnginePage::Feature::Geolocation:
			this->location = value;
			break;
		case QWebEnginePage::Feature::Notifications:
			this->notification = value;
			break;
		case QWebEnginePage::Feature::MediaAudioCapture:
			this->media_audio_capture = value;
			break;
		case QWebEnginePage::Feature::MediaVideoCapture:
			this->media_video_capture = value;
			break;
		case QWebEnginePage::Feature::MediaAudioVideoCapture:
			this->media_audio_capture = value;
			this->media_video_capture = value;
			break;
		case QWebEnginePage::Feature::DesktopVideoCapture:
			this->desktop_audio_video_capture = value;
			break;
		case QWebEnginePage::Feature::DesktopAudioVideoCapture:
			this->desktop_audio_video_capture = value;
			break;
	}
}
