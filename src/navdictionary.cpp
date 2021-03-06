/* Copyright(c) 2012 Optimal Bits Sweden AB. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "navdictionary.h"
#include "navutils.h"

using namespace v8;

Persistent<ObjectTemplate> NAVDictionary::constructor;

NAVDictionary::NAVDictionary(){
}

NAVDictionary::~NAVDictionary(){
}

void NAVDictionary::Init(v8::Isolate *isolate) {
  v8::Local<v8::ObjectTemplate> tpl = ObjectTemplate::New(isolate);
  tpl->SetInternalFieldCount(1);

  constructor.Reset(isolate, tpl);
}

v8::Local<v8::Object> NAVDictionary::New(v8::Isolate *isolate, AVDictionary *pDictionary) {
  v8::EscapableHandleScope scope(isolate);

  NAVDictionary *instance = new NAVDictionary();

  v8::Local<v8::ObjectTemplate> cons = v8::Local<v8::ObjectTemplate>::New(isolate, constructor);
  Handle<Object> obj = cons->NewInstance();
  instance->Wrap(obj);

  AVDictionaryEntry *tag = NULL;
  while ((tag = av_dict_get(pDictionary, "", tag, AV_DICT_IGNORE_SUFFIX))){
    SET_KEY_VALUE(obj, tag->key, String::NewFromUtf8(isolate, tag->value));
  }

  return scope.Escape(obj);
}

AVDictionary *NAVDictionary::New(v8::Local<v8::Object> obj){
  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  AVDictionary *pDict = NULL;

  Local<Array> properties = obj->GetOwnPropertyNames();
  unsigned int length = properties->Length();
  for(unsigned int i=0; i<length; i++){
    Local<Value> key = properties->Get(Integer::New(isolate, i));
    Local<Value> value = obj->Get(key);

    String::Utf8Value utf8Key(key);
    String::Utf8Value utf8Value(value);
    av_dict_set(&pDict, *utf8Key, *utf8Value, 0);
  }
  return pDict;
}

void NAVDictionary::Info(AVDictionary *pDictionary) {
  if(pDictionary){
    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(pDictionary, "", tag, AV_DICT_IGNORE_SUFFIX))){
      printf("Key: %s, Value: %s\n", tag->key, tag->value);
    }
  }
}
