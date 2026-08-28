#pragma once

#include <string>

class Layer {
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}

	inline const std::string& GetName() const { return debugName; }

protected:
	std::string debugName;
};