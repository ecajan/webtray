#ifndef PERMISSION_MANAGER_HPP
#define PERMISSION_MANAGER_HPP

#include <QtWebEngineCore/QWebEnginePage>
#include <filesystem>
#include <fstream>

class PermissionManager
{
private:
	std::string path;
	bool _lock_mouse = false;
	bool _location = false;
	bool _notification = false;
	bool _media_audio_capture = false;
	bool _media_video_capture = false;
	bool _desktop_video_capture = false;
	bool _desktop_audio_video_capture = false;

public:
	PermissionManager(std::string path);
	~PermissionManager();
	void save();
	bool get(QWebEnginePage::Feature feature);
	void set(QWebEnginePage::Feature feature, bool value);
};

#endif
