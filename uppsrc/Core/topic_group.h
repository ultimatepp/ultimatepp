//#BLITZ_APPROVE

#ifndef TOPIC_VERA
#define TOPIC_VERA 0
#define TOPIC_VERB 0
#else
	#if TOPIC_VERA == 0
		#undef TOPIC_VERA
		#define TOPIC_VERA 1
	#elif TOPIC_VERA == 1
		#undef TOPIC_VERA
		#define TOPIC_VERA 2
	#elif TOPIC_VERA == 2
		#undef TOPIC_VERA
		#define TOPIC_VERA 3
	#elif TOPIC_VERA == 3
		#undef TOPIC_VERA
		#define TOPIC_VERA 4
	#elif TOPIC_VERA == 4
		#undef TOPIC_VERA
		#define TOPIC_VERA 5
	#elif TOPIC_VERA == 5
		#undef TOPIC_VERA
		#define TOPIC_VERA 6
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 7
		#undef TOPIC_VERA
		#define TOPIC_VERA 8
	#elif TOPIC_VERA == 8
		#undef TOPIC_VERA
		#define TOPIC_VERA 9
	#elif TOPIC_VERA == 9
		#undef TOPIC_VERA
		#define TOPIC_VERA 0
		#if TOPIC_VERB == 0
			#undef TOPIC_VERB
			#define TOPIC_VERB 1
		#elif TOPIC_VERB == 1
			#undef TOPIC_VERB
			#define TOPIC_VERB 2
		#elif TOPIC_VERB == 2
			#undef TOPIC_VERB
			#define TOPIC_VERB 3
		#elif TOPIC_VERB == 3
			#undef TOPIC_VERB
			#define TOPIC_VERB 4
		#elif TOPIC_VERB == 4
			#undef TOPIC_VERB
			#define TOPIC_VERB 5
		#elif TOPIC_VERB == 5
			#undef TOPIC_VERB
			#define TOPIC_VERB 6
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 7
			#undef TOPIC_VERB
			#define TOPIC_VERB 8
		#elif TOPIC_VERB == 8
			#undef TOPIC_VERB
			#define TOPIC_VERB 9
		#elif TOPIC_VERB == 9
			#undef TOPIC_VERB
			#define TOPIC_VERB 10
		#elif TOPIC_VERB == 10
			#undef TOPIC_VERB
			#define TOPIC_VERB 11
		#elif TOPIC_VERB == 11
			#undef TOPIC_VERB
			#define TOPIC_VERB 12
		#elif TOPIC_VERB == 12
			#undef TOPIC_VERB
			#define TOPIC_VERB 13
		#elif TOPIC_VERB == 13
			#undef TOPIC_VERB
			#define TOPIC_VERB 14
		#elif TOPIC_VERB == 14
			#undef TOPIC_VERB
			#define TOPIC_VERB 15
		#elif TOPIC_VERB == 15
			#undef TOPIC_VERB
			#define TOPIC_VERB 16
		#elif TOPIC_VERB == 16
			#undef TOPIC_VERB
			#define TOPIC_VERB 17
		#elif TOPIC_VERB == 17
			#undef TOPIC_VERB
			#define TOPIC_VERB 18
		#elif TOPIC_VERB == 18
			#undef TOPIC_VERB
			#define TOPIC_VERB 19
		#elif TOPIC_VERB == 19
			#undef TOPIC_VERB
			#define TOPIC_VERB 20
		#else
			#error Too many topic files !
		#endif
	#endif
#endif

#define TOPIC(id)       { const char *topic = id;
#define TITLE(x)          const char *title = x;
#define COMPRESSED        static const UPP::byte data[] = {
#define END_TOPIC         }; RegisterTopic__(ASSTRING(TOPICFILE), topic, title, data, __countof(data)); }

INITBLOCK_(COMBINE3(TOPIC__, TOPIC_VERB, TOPIC_VERA))
{
	#include TOPICFILE
}

#undef TOPIC
#undef COMPRESSED
#undef END_TOPIC
#undef TOPICFILE
