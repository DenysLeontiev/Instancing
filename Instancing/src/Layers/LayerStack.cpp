#include "LayerStack.h"
#include <algorithm>

LayerStack::LayerStack() {
}

LayerStack::~LayerStack() {
	for (Layer* layer : layers) {
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer) {
	layers.push_back(layer);
	layer->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
	auto it = std::find(layers.begin(), layers.end(), layer);

	if (it != layers.end()) {
		layers.erase(it);
	}
}