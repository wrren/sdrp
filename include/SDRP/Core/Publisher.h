/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_PUBLISHER_H
#define RD_SDRP_PUBLISHER_H

#include <set>

namespace Radicle { namespace SDRP 
{
	/**
	 *	Utility class for containment of event subscribers in the observer
	 * 	pattern
	 */
	template<class T>
	class Publisher
	{
	public:

		/// Subscriber Container Type
		typedef std::set<T*> SubscriberContainer;

		/**
		 *	Subscribe to this publisher
		 * @param subscriber 	New Subscriber
		 */
		void Subscribe( T* subscriber )
		{
			m_subscribers.insert( subscriber );
		}

		/**
		 *	Unsubscribe from this publisher
		 * @param subscriber 	Subscriber to be unsubscribed
		 */
		void Unsubscribe( T* subscriber )
		{
			m_subscribers.erase( subscriber );
		}

	protected:

		/**
		 *	Get the subscribers for this publisher
		 */
		SubscriberContainer& Subscribers()
		{
			return m_subscribers;
		}
		const SubscriberContainer& Subscribers() const
		{
			return m_subscribers;
		}

		/**
		 *	Clear the subscriber container
		 */
		void Clear()
		{
			m_subscribers.clear();
		}

		/// Subscribers
		SubscriberContainer m_subscribers;
	};
}}

#endif // RD_SDRP_PUBLISHER_H
