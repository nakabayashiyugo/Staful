#include "ButtonManager.h"

#include "Button.h"

namespace ButtonManager
{
	std::vector<Button*> buttons_;
	//ボタンのタイプ
	BUTTONTYPE btnType_;
}
int ButtonManager::AddButton(std::string _btnName, GameObject* _parent)
{
	Button* btn = new Button(_btnName, _parent);
	btn->Initialize();
	if (_parent != nullptr)	_parent->PushBackChild(btn);
	buttons_.push_back(btn);
	return buttons_.size() - 1;
}

void ButtonManager::RemoveButton(int handle)
{
	buttons_.erase(buttons_.begin() + handle);
}

void ButtonManager::RemoveAllButton()
{
	for (int i = 0; i < buttons_.size(); i++)
	{
		RemoveButton(i);
	}
}

Button* ButtonManager::GetButton(int handle)
{
	return buttons_[handle];
}

void ButtonManager::SetPict(int _handle, int _imageHandle)
{
	buttons_[_handle]->SetPictNum(_imageHandle);
}

void ButtonManager::SetTransform(int _handle, Transform _transform)
{
	buttons_[_handle]->SetTransform(_transform);
}
