

#include "config.h"
#include "WebDataSourceImpl.h"

#include "ApplicationCacheHostInternal.h"
#include "WebURL.h"
#include "WebURLError.h"
#include "WebVector.h"

using namespace WebCore;

namespace WebKit {

WebPluginLoadObserver* WebDataSourceImpl::m_nextPluginLoadObserver = 0;

PassRefPtr<WebDataSourceImpl> WebDataSourceImpl::create(const ResourceRequest& request, const SubstituteData& data)
{
    return adoptRef(new WebDataSourceImpl(request, data));
}

const WebURLRequest& WebDataSourceImpl::originalRequest() const
{
    m_originalRequestWrapper.bind(DocumentLoader::originalRequest());
    return m_originalRequestWrapper;
}

const WebURLRequest& WebDataSourceImpl::request() const
{
    m_requestWrapper.bind(DocumentLoader::request());
    return m_requestWrapper;
}

const WebURLResponse& WebDataSourceImpl::response() const
{
    m_responseWrapper.bind(DocumentLoader::response());
    return m_responseWrapper;
}

bool WebDataSourceImpl::hasUnreachableURL() const
{
    return !DocumentLoader::unreachableURL().isEmpty();
}

WebURL WebDataSourceImpl::unreachableURL() const
{
    return DocumentLoader::unreachableURL();
}

void WebDataSourceImpl::redirectChain(WebVector<WebURL>& result) const
{
    result.assign(m_redirectChain);
}

WebString WebDataSourceImpl::pageTitle() const
{
    return title();
}

WebNavigationType WebDataSourceImpl::navigationType() const
{
    return toWebNavigationType(triggeringAction().type());
}

double WebDataSourceImpl::triggeringEventTime() const
{
    if (!triggeringAction().event())
        return 0.0;

    // DOMTimeStamp uses units of milliseconds.
    return triggeringAction().event()->timeStamp() / 1000.0;
}

WebDataSource::ExtraData* WebDataSourceImpl::extraData() const
{
    return m_extraData.get();
}

void WebDataSourceImpl::setExtraData(ExtraData* extraData)
{
    m_extraData.set(extraData);
}

WebApplicationCacheHost* WebDataSourceImpl::applicationCacheHost() {
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    return ApplicationCacheHostInternal::toWebApplicationCacheHost(DocumentLoader::applicationCacheHost());
#else
    return 0;
#endif
}

WebNavigationType WebDataSourceImpl::toWebNavigationType(NavigationType type)
{
    switch (type) {
    case NavigationTypeLinkClicked:
        return WebNavigationTypeLinkClicked;
    case NavigationTypeFormSubmitted:
        return WebNavigationTypeFormSubmitted;
    case NavigationTypeBackForward:
        return WebNavigationTypeBackForward;
    case NavigationTypeReload:
        return WebNavigationTypeReload;
    case NavigationTypeFormResubmitted:
        return WebNavigationTypeFormResubmitted;
    case NavigationTypeOther:
    default:
        return WebNavigationTypeOther;
    }
}

const KURL& WebDataSourceImpl::endOfRedirectChain() const
{
    ASSERT(!m_redirectChain.isEmpty());
    return m_redirectChain.last();
}

void WebDataSourceImpl::clearRedirectChain()
{
    m_redirectChain.clear();
}

void WebDataSourceImpl::appendRedirect(const KURL& url)
{
    m_redirectChain.append(url);
}

void WebDataSourceImpl::setNextPluginLoadObserver(PassOwnPtr<WebPluginLoadObserver> observer)
{
    // This call should always be followed up with the creation of a
    // WebDataSourceImpl, so we should never leak this object.
    m_nextPluginLoadObserver = observer.release();
}

WebDataSourceImpl::WebDataSourceImpl(const ResourceRequest& request, const SubstituteData& data)
    : DocumentLoader(request, data)
{
    if (m_nextPluginLoadObserver) {
        // When a new frame is created, it initially gets a data source for an
        // empty document.  Then it is navigated to the source URL of the
        // frame, which results in a second data source being created.  We want
        // to wait to attach the WebPluginLoadObserver to that data source.
        if (!request.url().isEmpty()) {
            ASSERT(m_nextPluginLoadObserver->url() == request.url());
            m_pluginLoadObserver.set(m_nextPluginLoadObserver);
            m_nextPluginLoadObserver = 0;
        }
    }
}

WebDataSourceImpl::~WebDataSourceImpl()
{
}

} // namespace WebKit
