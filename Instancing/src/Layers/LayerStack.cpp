#include "LayerStack.h"

LayerStack::LayerStack() {
	layerInsert = layers.begin();
}

LayerStack::~LayerStack() {
	for (Layer* layer : layers) {
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer) {
	layerInsert = layers.emplace(layerInsert, layer);
	layer->OnAttach();
}

void LayerStack::PopLayer(Layer* layer) {
	auto it = std::find(layers.begin(), layers.end(), layer);

	if (it != layers.end()) {
		layers.erase(it);
		layerInsert--;
	}
}