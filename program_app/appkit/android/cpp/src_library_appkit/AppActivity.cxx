#include "cqjnihelper.hh"

extern "C" void _cq_cpp_entry();

extern "C" JNIEXPORT void JNICALL Java_src_library_appkit_AppActivity_entry
    (JNIEnv *, jobject)
{
    _cq_cpp_entry();
}