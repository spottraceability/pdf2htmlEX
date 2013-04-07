/*
 * HTMLTextPage.cc
 *
 * Generate and optimized HTML for one Page
 *
 * Copyright (C) 2013 Lu Wang <coolwanglu@gmail.com>
 */

#include <algorithm>

#include "HTMLTextPage.h"

namespace pdf2htmlEX {

using std::ostream;
using std::unique_ptr;

HTMLTextPage::HTMLTextPage(const Param & param, AllStateManager & all_manager)
    : param(param)
    , all_manager(all_manager)
    , last_line(nullptr)
{ } 

void HTMLTextPage::dump_text(ostream & out)
{
    prepare();
    for(auto iter = text_lines.begin(); iter != text_lines.end(); ++iter)
        (*iter)->dump_text(out);
}

void HTMLTextPage::append_unicodes(const Unicode * u, int l)
{
    if(!last_line)
        open_new_line();
    last_line->append_unicodes(u, l);
}

void HTMLTextPage::append_offset(double offset)
{
    if(!last_line)
        open_new_line();
    last_line->append_offset(offset);
}

void HTMLTextPage::append_state(const HTMLState & state)
{
    if(!last_line)
        open_new_line();
    last_line->append_state(state);
}

void HTMLTextPage::dump_css(ostream & out)
{
    //TODO
}

void HTMLTextPage::clear(void)
{
    text_lines.clear();
    last_line = nullptr;
}

void HTMLTextPage::open_new_line(void)
{
    if(last_line && (last_line->empty()))
    {
        // state and offsets might be nonempty
        last_line->clear();
    }
    else
    {
        text_lines.emplace_back(new HTMLTextLine(param, all_manager));
        last_line = text_lines.back().get();
    }
}

static bool is_text_line_empty(const unique_ptr<HTMLTextLine>& p)
{
    return p->empty();
}

void HTMLTextPage::prepare(void)
{
    // remove empty lines
    text_lines.erase(remove_if(text_lines.begin(), text_lines.end(), is_text_line_empty), text_lines.end());
    for(auto iter = text_lines.begin(); iter != text_lines.end(); ++iter)
        (*iter)->prepare();
    if(param.optimize_text)
        optimize();
}

void HTMLTextPage::optimize(void)
{
    //TODO
}

} // namespace pdf2htmlEX