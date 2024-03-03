#ifndef PERMISSION_MANAGER_HPP
#define PERMISSION_MANAGER_HPP

#include <QtWebEngineCore/QWebEnginePage>
#include <filesystem>
#include <fstream>

class PermissionManager
{
private:
	std::string path;
	bool lock_mouse = false;
	bool location = false;
	bool notification = false;
	bool media_audio_capture = false;
	bool media_video_capture = false;
	bool desktop_audio_video_capture = false;

public:
	PermissionManager(std::string path);
	~PermissionManager();
	void save();
	bool get(QWebEnginePage::Feature feature);
	void set(QWebEnginePage::Feature feature, bool value);
};

#endif
