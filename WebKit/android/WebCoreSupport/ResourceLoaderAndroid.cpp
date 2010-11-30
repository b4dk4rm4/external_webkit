/*
 * Copyright 2009, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <config.h>
#include <ResourceLoaderAndroid.h>

#include "FrameLoaderClientAndroid.h"
#include "WebCoreFrameBridge.h"
#include "WebCoreResourceLoader.h"
#include "WebUrlLoader.h"

using namespace android;

namespace WebCore {

PassRefPtr<ResourceLoaderAndroid> ResourceLoaderAndroid::start(
        ResourceHandle* handle, const ResourceRequest& request, FrameLoaderClient* client, bool isMainResource, bool isSync, bool isPrivateBrowsing)
{
    // Called on main thread
#if USE(CHROME_NETWORK_STACK)
    return WebUrlLoader::start(client, handle, request, isSync, isPrivateBrowsing);
#else
    FrameLoaderClientAndroid* clientAndroid = static_cast<FrameLoaderClientAndroid*> (client);
    return clientAndroid->webFrame()->startLoadingResource(handle, request, isMainResource, isSync);
#endif
}

bool ResourceLoaderAndroid::willLoadFromCache(const WebCore::KURL& url, int64_t identifier)
{
#if USE(CHROME_NETWORK_STACK)
    // This method is used to determine if a POST request can be repeated from
    // cache, but you cannot really know until you actually try to read from the
    // cache.  Even if we checked now, something else could come along and wipe
    // out the cache entry by the time we fetch it.
    //
    // So, we always say yes here, to prevent the FrameLoader from initiating a
    // reload.  Then in FrameLoaderClientImpl::dispatchWillSendRequest, we
    // fix-up the cache policy of the request to force a load from the cache.
    return true;
#else
    return WebCoreResourceLoader::willLoadFromCache(url, identifier);
#endif
}

}
