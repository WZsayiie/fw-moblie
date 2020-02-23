#include "JNI.hh"
#include "cqjnihelper.hh"

static jclass clazz() {
    static jclass clazz = nullptr;
    if (clazz == nullptr) {
        cqJNIEnv::findClass(&clazz, cqJNIEnv::env(), "src/library/basis/JNI");
    }
    return clazz;
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeBytesSuck
    (JNIEnv *env, jclass, jobject _send, jobject _src)
{
    static jmethodID method = nullptr;
    cqJNIEnv::staticMethod(&method, env, clazz(), "onBytesSuck", "([B)V");

    auto send = cqJNIType::ptr<cq_bytes_send>(_send);
    auto src  = cqJNIType::ptr<cq_bytes *   >(_src );

    std::vector<uint8_t>   object = cq_cpp::from(send, src);
    cqJNILocal<jbyteArray> array  = cqJNIType::jniData(env, object);

    env->CallStaticVoidMethod(clazz(), method, array.get());
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeLongsSuck
    (JNIEnv *, jclass, jobject _send, jobject _src)
{
    static jmethodID methodID = nullptr;
    cqJNIStatic method(clazz(), &methodID, "onLongsSuck");

    auto send = cqJNIType::ptr<cq_int64s_send>(_send);
    auto src  = cqJNIType::ptr<cq_int64s *   >(_src );

    std::vector<int64_t> object = cq_cpp::from(send, src);
    for (auto &it : object) {
        method.push(it);
        method.callVoid();
    }
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeStringsSuck
    (JNIEnv *, jclass, jobject _send, jobject _src)
{
    static jmethodID methodID = nullptr;
    cqJNIStatic method(clazz(), &methodID, "onStringsSuck");

    auto send = cqJNIType::ptr<cq_strings_send>(_send);
    auto src  = cqJNIType::ptr<cq_strings *   >(_src );

    std::vector<std::string> object = cq_cpp::from(send, src);
    for (auto &it : object) {
        method.push(it.c_str());
        method.callVoid();
    }
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeSSMapSuck
    (JNIEnv *, jclass, jobject _send, jobject _src)
{
    static jmethodID methodID = nullptr;
    cqJNIStatic method(clazz(), &methodID, "onSSMapSuck");

    auto send = cqJNIType::ptr<cq_ss_map_send>(_send);
    auto src  = cqJNIType::ptr<cq_ss_map *   >(_src );

    std::map<std::string, std::string> object = cq_cpp::from(send, src);
    for (auto &cp : object) {
        method.push(cp.first .c_str());
        method.push(cp.second.c_str());
        method.callVoid();
    }
}

template<class T> auto cached(T fn) -> decltype(fn()) {
    static thread_local decltype(fn()) object = nullptr;
    if (object == nullptr) {
        object = new typename std::remove_reference<decltype(*object)>::type;
    }
    return object;
}

static std::vector<int64_t>               *cached_int64s () {return cached(cached_int64s );}
static std::vector<std::string>           *cached_strings() {return cached(cached_strings);}
static std::map<std::string, std::string> *cached_ss_map () {return cached(cached_ss_map );}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeLongsAdd
    (JNIEnv *, jclass, jlong _value)
{
    auto value = (int64_t)_value;
    cached_int64s()->push_back(value);
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeStringsAdd
    (JNIEnv *env, jclass, jstring _value)
{
    std::string string = cqJNIType::string(env, _value);
    cached_strings()->push_back(string);
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeSSMapAdd
    (JNIEnv *env, jclass, jstring _key, jstring _value)
{
    std::string key   = cqJNIType::string(env, _key  );
    std::string value = cqJNIType::string(env, _value);

    (*cached_ss_map())[key] =value;
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeBytesSend
    (JNIEnv *env, jclass, jbyteArray _bytes, jobject _receive, jobject _dst)
{
    auto bytes   = cqJNIType::data(env, _bytes);
    auto receive = cqJNIType::ptr<cq_bytes_recv>(_receive);
    auto dst     = cqJNIType::ptr<cq_bytes *   >(_dst    );

    cq_cpp::send(bytes, receive, dst);
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeLongsSend
    (JNIEnv *, jclass, jobject _receive, jobject _dst)
{
    auto receive = cqJNIType::ptr<cq_int64s_recv>(_receive);
    auto dst     = cqJNIType::ptr<cq_int64s *   >(_dst    );

    std::vector<int64_t> *object = cached_int64s();
    cq_cpp::send(*object, receive, dst);
    object->clear();
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeStringsSend
    (JNIEnv *, jclass, jobject _receive, jobject _dst)
{
    auto receive = cqJNIType::ptr<cq_strings_recv>(_receive);
    auto dst     = cqJNIType::ptr<cq_strings *   >(_dst    );

    std::vector<std::string> *object = cached_strings();
    cq_cpp::send(*object, receive, dst);
    object->clear();
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_JNI_makeSSMapSend
    (JNIEnv *, jclass, jobject _receive, jobject _dst)
{
    auto receive = cqJNIType::ptr<cq_ss_map_recv>(_receive);
    auto dst     = cqJNIType::ptr<cq_ss_map *   >(_dst    );

    std::map<std::string, std::string> *object = cached_ss_map();
    cq_cpp::send(*object, receive, dst);
    object->clear();
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_JNI_makeBytesStore
    (JNIEnv *env, jclass, jbyteArray bytes)
{
    std::vector<uint8_t> object = cqJNIType::data(env, bytes);
    cq_bytes *ret = cq_cpp::store(object);

    return cqJNIType::jniPtr(ret);
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_JNI_makeLongsStore
    (JNIEnv *, jclass)
{
    std::vector<int64_t> *object = cached_int64s();
    cq_int64s *ret = cq_cpp::store(*object);
    object->clear();

    return cqJNIType::jniPtr(ret);
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_JNI_makeStringsStore
    (JNIEnv *, jclass)
{
    std::vector<std::string> *object = cached_strings();
    cq_strings *ret = cq_cpp::store(*object);
    object->clear();

    return cqJNIType::jniPtr(ret);
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_JNI_makeSSMapStore
    (JNIEnv *, jclass)
{
    std::map<std::string, std::string> *object = cached_ss_map();
    cq_ss_map *ret = cq_cpp::store(*object);
    object->clear();

    return cqJNIType::jniPtr(ret);
}
