#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <any>
#include <string>
#include <functional>

#define CLASS_PROPERTY(type, name, defaultValue) Property<type, defaultValue> name = Property<type, defaultValue>(#name, this);
#define PROPERTY(type, name, defaultValue) Property<type, defaultValue> name = Property<type, defaultValue>(#name);

template <class T>	
using callback_t = std::function<void (const T&)>;

class PropertyHandler;

class PropertyInterface {
	public:
		virtual void bind(const std::any& callback) = 0;

		virtual void unbindAll() = 0;
};

template <class T, T defaultValue>
class Property : public PropertyInterface {
	public:
		Property(const char* name) : name(name), data(defaultValue) {};

		Property(const char* name, PropertyHandler* handler);

		inline const T& getValue() const {
			return data;
		};

		inline void setValue(const T& val) {
			data = val;

			// Notify for change
			for (auto& callback : callbacks) {
				callback(data);
			}
		};

		inline virtual void bind(const std::any& callback) {
			const callback_t<T>& cast_callback = std::any_cast<callback_t<T>>(callback);
			callbacks.push_back(cast_callback);
		};

		inline virtual void unbindAll() {
			callbacks.clear();
		};

		__declspec(property(get = getValue, put = setValue)) T value;

	protected:

	private:
		const char* name;
		T data;
		std::vector<callback_t<T>> callbacks;
};

class PropertyHandler {
	public:
		template <class T, T defaultValue>
		friend Property<T, defaultValue>::Property(const char* name, PropertyHandler* handler);

		inline void bind(const char* name, const std::any& callback) {
			properties[name]->bind(callback);
		};

		inline void unbindAll(const char* name) {
			properties[name]->unbindAll();
		};

	private:
		inline void declare(const char* name, PropertyInterface* p) {
			properties[name] = p;
		};

		std::unordered_map<const char*, PropertyInterface*> properties;
};


template<class T, T defaultValue>
inline Property<T, defaultValue>::Property(const char* name, PropertyHandler* handler)
	: name(name), data(defaultValue)
{
	handler->declare(name, this);
}