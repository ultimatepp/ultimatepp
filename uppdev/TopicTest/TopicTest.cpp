#include <Topic/Topic.h>

#define TOPICFILE <TopicTest/res.tpp/all.i>
#include <Core/topic_group.h>

GUI_APP_MAIN
{
	DUMP(Format(GetTopic("topic://TopicTest/res/test$IT-IT").text, "Test"));
}
