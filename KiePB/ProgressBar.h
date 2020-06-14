//
// Created by kie on 2020/6/9.
//

#ifndef KIEPB_PROGRESSBAR_H
#define KIEPB_PROGRESSBAR_H

// INCLUDE AREA
#include <iostream>
#include <memory> // Always use smart pointer to ensure memory safety :)
#include <string>
#include <codecvt>
#include <utility>
#include <sstream>
#include <iomanip>
#include <sstream>

#ifdef __APPLE__
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#endif
#ifdef __linux__
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#endif

// INCLUDE AREA END

namespace Kie {


    enum class Color{
        Default,Black,Red,Green,Yellow,Blue,Magenta,Cyan,White,BrightBlack,BrightRed,BrightGreen,BrightYellow,
        BrightBlue,BrightMagenta,BrightCyan,BrightWhite
    };

    enum class ProcessType{
        UsedTime,                              // 3:10
        Time,                                  // 3:10/4:40
        ProcessedFile,                         // 3
        File,                                  // 3/103
    };

    enum class StyleType{
        Style1,                                // The simplest
        Style2,
        Style3,
        Style4,
        Style5,
    };

#if defined(__APPLE__) || defined(__linux__)
    inline
#endif
    namespace util_unix{
        using namespace std::string_literals;
        std::string wrap_color(Color color,const std::string& content){
            switch (color) {
                case Color::Default:
                    return content;
                case Color::Black:
                    return "\033[30m"s+content+"\033[m";
                case Color::Red:
                    return "\033[31m"s+content+"\033[m";
                case Color::Green:
                    return "\033[32m"s+content+"\033[m";
                case Color::Yellow:
                    return "\033[33m"s+content+"\033[m";
                case Color::Blue:
                    return "\033[34m"s+content+"\033[m";
                case Color::Magenta:
                    return "\033[35m"s+content+"\033[m";
                case Color::Cyan:
                    return "\033[36m"s+content+"\033[m";
                case Color::White:
                    return "\033[37m"s+content+"\033[m";
                case Color::BrightBlack:
                    return "\033[90m"s+content+"\033[m";
                case Color::BrightRed:
                    return "\033[91m"s+content+"\033[m";
                case Color::BrightGreen:
                    return "\033[92m"s+content+"\033[m";
                case Color::BrightYellow:
                    return "\033[93m"s+content+"\033[m";
                case Color::BrightBlue:
                    return "\033[94m"s+content+"\033[m";
                case Color::BrightMagenta:
                    return "\033[95m"s+content+"\033[m";
                case Color::BrightCyan:
                    return "\033[96m"s+content+"\033[m";
                case Color::BrightWhite:
                    return "\033[97m"s+content+"\033[m";
            }
            return content;
        }

        unsigned int window_width(){
            winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            return w.ws_col;
        }
    }

#if defined(_WIN32)
    inline
#endif
    namespace util_windows{

    }

    inline namespace util {
        /**
        * Come on, I am looking forward to C++20, which has generator and coroutine.
        * But for now, I have to implement by myself.
        */
        class EmojiGenerator {
            static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
            int current_index = 0;
            std::wstring emoji = L"";
        public:
            EmojiGenerator(std::wstring emoji) : emoji(std::move(emoji)) {}

            std::string get() {
                if (emoji.empty()) return "";
                if (current_index < emoji.size()) return convert.to_bytes(emoji[current_index++]);
                else {
                    current_index = 0;
                    return convert.to_bytes(emoji[current_index++]);
                }
            }

            EmojiGenerator &operator=(const std::wstring &_emoji) {
                emoji = _emoji;
                return *this;
            }
        };

        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> EmojiGenerator::convert = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{};


        std::string format_time(std::chrono::seconds sec) {
            std::stringstream ss;
            auto c = sec.count();
            ss << std::setfill('0') << (c / 60 / 60) << ":" << std::setw(2) << (c / 60) % 60 << ":" << std::setw(2)
               << c % 60;
            return ss.str();
        }

        std::string format_time(unsigned long long sec) {
            return format_time(std::chrono::seconds(sec));
        }

        template<typename T,typename = void>
        class HasLength:public std::false_type {};

        template<typename T>
        class HasLength<T,std::void_t<decltype(std::declval<T>().length())>>: public std::true_type {};

        template<typename T,typename = void>
        class HasSize:public std::false_type {};

        template<typename T>
        class HasSize<T,std::void_t<decltype(std::declval<T>().size())>>: public std::true_type {};

    }


    /*
     * A progress bar is consisted with the following content
     * [start_text][start_emoji][left_boundary][past_sign][current_sign][future_sign][right_boundary][ProcessType][trail_text][trail_emoji]
     */
    class ProgressBar {

        unsigned int total = 0;
        unsigned int current = 0;
        int step = 1;
        unsigned int bar_width = 50;
        unsigned int files = 0;

        unsigned int time_duration = 200;
        std::chrono::steady_clock::time_point last_time = std::chrono::time_point<std::chrono::steady_clock>();

        std::string left_boundary = "[";
        std::string right_boundary = "]";
        std::string past_sign = "=";
        std::string current_sign = ">";
        std::string future_sign = "-";

        std::string start_processing_text = "";
        std::string start_finish_text = "";
        std::string trail_processing_text = "";
        std::string trail_finish_text = "";

        EmojiGenerator start_processing_emoji = std::wstring(L"");
        EmojiGenerator start_finish_emoji = std::wstring(L"");
        EmojiGenerator trail_processing_emoji = std::wstring(L"");
        EmojiGenerator trail_finish_emoji = std::wstring(L"");

        ProcessType type = ProcessType::Time;
        Color start_processing_text_color = Color::Default;
        Color start_finish_text_color = Color::Default;
        Color trail_processing_text_color = Color::Default;
        Color trail_finish_text_color = Color::Default;
        Color left_boundary_color = Color::Default;
        Color right_boundary_color = Color::Default;
        Color past_sign_color = Color::Default;
        Color current_sign_color = Color::Default;
        Color future_sign_color = Color::Default;
        Color type_color = Color::Default;

        std::chrono::steady_clock::time_point start;
        std::chrono::seconds used_seconds;

        std::stringstream ss;

        //Getter and setter
    public:
        unsigned int getTotal() const {
            return total;
        }

        void setTotal(unsigned int total) {
            ProgressBar::total = total;
        }

        unsigned int getCurrent() const {
            return current;
        }

        void setCurrent(unsigned int current) {
            ProgressBar::current = current;
        }

        int getStep() const {
            return step;
        }

        void setStep(int step) {
            ProgressBar::step = step;
        }

        unsigned int getBarWidth() const {
            return bar_width;
        }

        void setBarWidth(unsigned int barWidth) {
            bar_width = barWidth;
        }

        unsigned int getTimeDuration() const {
            return time_duration;
        }

        void setTimeDuration(unsigned int timeDuration) {
            time_duration = timeDuration;
        }

        const std::string &getLeftBoundary() const {
            return left_boundary;
        }

        void setLeftBoundary(const std::string &leftBoundary) {
            left_boundary = leftBoundary;
        }

        const std::string &getRightBoundary() const {
            return right_boundary;
        }

        void setRightBoundary(const std::string &rightBoundary) {
            right_boundary = rightBoundary;
        }

        const std::string &getPastSign() const {
            return past_sign;
        }

        void setPastSign(const std::string &pastSign) {
            past_sign = pastSign;
        }

        const std::string &getCurrentSign() const {
            return current_sign;
        }

        void setCurrentSign(const std::string &currentSign) {
            current_sign = currentSign;
        }

        const std::string &getFutureSign() const {
            return future_sign;
        }

        void setFutureSign(const std::string &futureSign) {
            future_sign = futureSign;
        }

        const std::string &getStartProcessingText() const {
            return start_processing_text;
        }

        void setStartProcessingText(const std::string &startProcessingText) {
            start_processing_text = startProcessingText;
        }

        const std::string &getStartFinishText() const {
            return start_finish_text;
        }

        void setStartFinishText(const std::string &startFinishText) {
            start_finish_text = startFinishText;
        }

        const std::string &getTrailProcessingText() const {
            return trail_processing_text;
        }

        void setTrailProcessingText(const std::string &trailProcessingText) {
            trail_processing_text = trailProcessingText;
        }

        const std::string &getTrailFinishText() const {
            return trail_finish_text;
        }

        void setTrailFinishText(const std::string &trailFinishText) {
            trail_finish_text = trailFinishText;
        }

        const EmojiGenerator &getStartProcessingEmoji() const {
            return start_processing_emoji;
        }

        void setStartProcessingEmoji(const EmojiGenerator &startProcessingEmoji) {
            start_processing_emoji = startProcessingEmoji;
        }

        const EmojiGenerator &getStartFinishEmoji() const {
            return start_finish_emoji;
        }

        void setStartFinishEmoji(const EmojiGenerator &startFinishEmoji) {
            start_finish_emoji = startFinishEmoji;
        }

        const EmojiGenerator &getTrailProcessingEmoji() const {
            return trail_processing_emoji;
        }

        void setTrailProcessingEmoji(const EmojiGenerator &trailProcessingEmoji) {
            trail_processing_emoji = trailProcessingEmoji;
        }

        const EmojiGenerator &getTrailFinishEmoji() const {
            return trail_finish_emoji;
        }

        void setTrailFinishEmoji(const EmojiGenerator &trailFinishEmoji) {
            trail_finish_emoji = trailFinishEmoji;
        }

        ProcessType getType() const {
            return type;
        }

        void setType(ProcessType type) {
            ProgressBar::type = type;
        }

        Color getStartProcessingTextColor() const {
            return start_processing_text_color;
        }

        void setStartProcessingTextColor(Color startProcessingTextColor) {
            start_processing_text_color = startProcessingTextColor;
        }

        Color getStartFinishTextColor() const {
            return start_finish_text_color;
        }

        void setStartFinishTextColor(Color startFinishTextColor) {
            start_finish_text_color = startFinishTextColor;
        }

        Color getTrailProcessingTextColor() const {
            return trail_processing_text_color;
        }

        void setTrailProcessingTextColor(Color trailProcessingTextColor) {
            trail_processing_text_color = trailProcessingTextColor;
        }

        Color getTrailFinishTextColor() const {
            return trail_finish_text_color;
        }

        void setTrailFinishTextColor(Color trailFinishTextColor) {
            trail_finish_text_color = trailFinishTextColor;
        }

        Color getLeftBoundaryColor() const {
            return left_boundary_color;
        }

        void setLeftBoundaryColor(Color leftBoundaryColor) {
            left_boundary_color = leftBoundaryColor;
        }

        Color getRightBoundaryColor() const {
            return right_boundary_color;
        }

        void setRightBoundaryColor(Color rightBoundaryColor) {
            right_boundary_color = rightBoundaryColor;
        }

        Color getPastSignColor() const {
            return past_sign_color;
        }

        void setPastSignColor(Color pastSignColor) {
            past_sign_color = pastSignColor;
        }

        Color getCurrentSignColor() const {
            return current_sign_color;
        }

        void setCurrentSignColor(Color currentSignColor) {
            current_sign_color = currentSignColor;
        }

        Color getFutureSignColor() const {
            return future_sign_color;
        }

        void setFutureSignColor(Color futureSignColor) {
            future_sign_color = futureSignColor;
        }

        Color getTypeColor() const {
            return type_color;
        }

        void setTypeColor(Color typeColor) {
            type_color = typeColor;
        }

    public:
        // Constructor
        ProgressBar(): ProgressBar(StyleType::Style1){}

        explicit ProgressBar(const StyleType& styleType){
            setStyleType(styleType);
            int rest_length = std::max(start_processing_text.length(),start_finish_text.length())+
                    2+
                    left_boundary.length()+
                    current_sign.length()+
                    right_boundary.length()+
                    std::max(std::to_string(files).length()*2+1, static_cast<std::size_t>(15))+
                    std::max(trail_processing_text.length(),trail_finish_text.length())+
                    2;

            bar_width = std::min(window_width()-rest_length, static_cast<unsigned int>(100));
        }

        /**
         * I need concept in C++20
         * @tparam T
         * @param v
         */
        template<typename T,std::enable_if_t<HasSize<T>::value || HasLength<T>::value>* = nullptr>
        explicit ProgressBar(T v){
            if constexpr(HasSize<T>::value){
                files = v.size();
                total = files;
            }else if constexpr(HasLength<T>::value){
                files = v.length();
                total = files;
            }
            setStyleType(StyleType::Style5);
            int rest_length = std::max(start_processing_text.length(),start_finish_text.length())+
                              2+
                              left_boundary.length()+
                              right_boundary.length()+
                              std::max(std::to_string(files).length()*2+1, static_cast<std::size_t>(15))+
                              std::max(trail_processing_text.length(),trail_finish_text.length())+
                              2;

            bar_width = std::min(window_width()-rest_length, static_cast<unsigned int>(100));
        }

        // Style
        void setStyleType(const StyleType& styleType){
            switch (styleType) {
                case StyleType::Style2:
                    past_sign = "█";
                    current_sign = "▏▎▍▋▊▉█";
                    future_sign = " ";
                    trail_finish_text = "·········ok";
                    break;
                case StyleType::Style3:
                    past_sign = "█";
                    current_sign = "█";
                    future_sign = "░";
                    trail_finish_text = "·········ok";
                    break;
                case StyleType::Style4:
                    start_processing_text = "PROCESSING";
                    start_finish_text     = "*FINISHED*";
                    start_processing_text_color = Color::Red;
                    start_finish_text_color = Color::Blue;
                    start_processing_emoji = L"🌑🌒🌓🌔🌕🌖🌗🌘";
                    trail_finish_text = "······ok";
                    start_finish_emoji = L"😃";
                    trail_finish_emoji = L"✅";
                    break;
                case StyleType::Style5:
                    start_processing_text = "PROCESSING";
                    start_finish_text     = "*FINISHED*";
                    start_processing_text_color = Color::Red;
                    start_finish_text_color = Color::Blue;
                    start_processing_emoji = L"🌑🌒🌓🌔🌕🌖🌗🌘";
                    start_finish_emoji = L"😃";
                    trail_finish_emoji = L"✅";
                    type = ProcessType::File;

                default:
                    // Only mean that I have consider this
                    break;
            }
        }


        // Simple usage
    public:

        bool is_finished(){
            return current>=total;
        }

        bool display(){
            if(!check_params_valid()){
                return false;
            }
            if(is_finished()){
                std::cout<<std::endl;
                return false;
            }
            auto now = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()-last_time.time_since_epoch()).count()<time_duration){
                return false;
            }
            next();
            std::cout<<'\r';
            display_start_text();
            display_start_emoji();
            display_left_boundary();
            display_past_sign();
            display_current_sign();
            display_future_sign();
            display_right_boundary();
            display_ProcessType();
            display_trail_text();
            display_trail_emoji();
            std::cout<<ss.str();
            std::cout.flush();
            ss = std::stringstream();
            return true;
        }

    private:

        void next(){
            if(current==0){
                start=std::chrono::steady_clock::now();
            }
            if(current<total){
                used_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-start);
            }
            current+=step;
        }

        bool check_params_valid(){
            // Total cannot be zero
            if(total==0) return false;
            if(step==0) return false;
            return true;
        }

        void display_start_text(){
            if(is_finished()){
                ss<<wrap_color(start_finish_text_color,start_finish_text);
            }else{
                ss<<wrap_color(start_processing_text_color,start_processing_text);
            }
        }

        void display_start_emoji(){
            if(is_finished()){
                ss<<start_finish_emoji.get();
            }else{
                ss<<start_processing_emoji.get();
            }
        }

        void display_left_boundary(){
            ss<<wrap_color(left_boundary_color,left_boundary);
        }

        void display_past_sign(){
            std::size_t count = current*bar_width/total;
            for(int i =0;i<count;i++){
                ss<<wrap_color(past_sign_color,past_sign);
            }
        }

        void display_current_sign(){
            if(current==0){
                ss<<wrap_color(future_sign_color,future_sign);
            }else if(current>=total){
                ss<<wrap_color(past_sign_color,past_sign);
            }else{
                ss<<wrap_color(current_sign_color,current_sign);
            }
        }

        void display_future_sign(){
            std::size_t count = bar_width-current*bar_width/total;
            for(int i=0;i<count;i++){
                ss<<wrap_color(future_sign_color,future_sign);
            }
        }

        void display_right_boundary(){
            ss<<wrap_color(right_boundary_color,right_boundary);
        }

        void display_ProcessType(){
            unsigned long long total_time;
            switch (type) {
                case ProcessType::UsedTime:
                    ss<<wrap_color(type_color,format_time(used_seconds));
                    break;
                case ProcessType::Time:
                    total_time = used_seconds.count()*total/current;
                    ss<<wrap_color(type_color,format_time(used_seconds)+"/"+format_time(total_time));
                    break;
                case ProcessType::ProcessedFile:
                    ss<<wrap_color(type_color,std::to_string(current/step));
                    break;
                case ProcessType::File:
                    if(files!=0){
                        ss<<wrap_color(type_color,std::to_string(current*files/total)+"/"+std::to_string(files));
                    }else{
                        ss<<wrap_color(type_color,std::to_string(current/step));
                    }
                    break;
            }
        }

        void display_trail_text(){
            if(is_finished()){
                ss<<wrap_color(trail_finish_text_color,trail_finish_text);
            }else{
                ss<<wrap_color(trail_processing_text_color,trail_processing_text);
            }
        }

        void display_trail_emoji(){
            if(is_finished()){
                ss<<trail_finish_emoji.get();
            }else{
                ss<<trail_processing_emoji.get();
            }
        }


    };

}


#endif //KIEPB_PROGRESSBAR_H
