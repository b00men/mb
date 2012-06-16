#ifndef DATA_FORUMS_H
#define DATA_FORUMS_H

#include <data/master.h>

namespace data {

        struct new_topic_form : public cppcms::form {
                cppcms::widgets::text title;
                cppcms::widgets::text author;
                cppcms::widgets::textarea comment;
                cppcms::widgets::file image;
                cppcms::widgets::submit submit;
                new_topic_form();
        };


        class forums : public master{
        public:
                forums() : next(0),prev(0),prevneg(0) {}
                struct topic {
                        topic() : id(0) {}
                        std::string title;
                        int id;
                };
                std::vector<topic> topics;
                int next,prev,prevneg;
                new_topic_form form;
        };

}

#endif
