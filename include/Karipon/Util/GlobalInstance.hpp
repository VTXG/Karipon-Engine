#pragma once

#include <revolution/types.h>

namespace krp {
    template < typename T >
    class GlobalInstance {
    public:
        static T* sInstance;

        static void initInstance() {
            if (sInstance == nullptr) {
                sInstance = new T();
            }
        }

        static T* getInstance() {
            return sInstance;
        }
    };

    template < typename T >
    T* GlobalInstance< T >::sInstance;
}; // namespace krp
