//
// Created by kie on 2020/6/9.
//

#ifndef KIEPB_PROGRESSBAR_H
#define KIEPB_PROGRESSBAR_H

// INCLUDE AREA
#include <iostream>
#include <memory> // Always use smart pointer to ensure memory safety :)
#include <string>
// INCLUDE AREA END

namespace Kie {


    enum class Color{
        Default,Black,Red,Green,Yellow,Blue,Magenta,Cyan,White,BrightBlack,BrightRed,BrightGreen
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
    };

    class ProgressBar {

        unsigned int total = 0;
        unsigned int current = 0;
        int step = 1;
        unsigned int width = 70;

        std::string left_boundary = "[";
        std::string right_boundary = "]";
        std::string past_sign = "=";
        std::string current_sign = ">";
        std::string future_sign = "-";

        std::string start_processing_emoji = "";
        std::string start_finish_emoji = "";
        std::string trail_processing_emoji = "";
        std::string trail_finish_emoji = "";
        bool use_emoji = false;

    public:
        // Getter and setter of the fields
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

        unsigned int getWidth() const {
            return width;
        }

        void setWidth(unsigned int width) {
            ProgressBar::width = width;
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

        const std::string &getStartProcessingEmoji() const {
            return start_processing_emoji;
        }

        void setStartProcessingEmoji(const std::string &startProcessingEmoji) {
            start_processing_emoji = startProcessingEmoji;
        }

        const std::string &getStartFinishEmoji() const {
            return start_finish_emoji;
        }

        void setStartFinishEmoji(const std::string &startFinishEmoji) {
            start_finish_emoji = startFinishEmoji;
        }

        const std::string &getTrailProcessingEmoji() const {
            return trail_processing_emoji;
        }

        void setTrailProcessingEmoji(const std::string &trailProcessingEmoji) {
            trail_processing_emoji = trailProcessingEmoji;
        }

        const std::string &getTrailFinishEmoji() const {
            return trail_finish_emoji;
        }

        void setTrailFinishEmoji(const std::string &trailFinishEmoji) {
            trail_finish_emoji = trailFinishEmoji;
        }

        bool isUseEmoji() const {
            return use_emoji;
        }

        void setUseEmoji(bool useEmoji) {
            use_emoji = useEmoji;
        }

        bool isUseEmojiSequence() const {
            return use_emoji_sequence;
        }

        void setUseEmojiSequence(bool useEmojiSequence) {
            use_emoji_sequence = useEmojiSequence;
        }

        ProcessType getType() const {
            return type;
        }

        void setType(ProcessType type) {
            ProgressBar::type = type;
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

    private:
        bool use_emoji_sequence = false;

        ProcessType type = ProcessType::Time;
        Color left_boundary_color = Color::Default;
        Color right_boundary_color = Color::Default;
        Color past_sign_color = Color::Default;
        Color current_sign_color = Color::Default;
        Color future_sign_color = Color::Default;

    public:
        // Constructor
        ProgressBar() = default;

        explicit ProgressBar(const StyleType& styleType){
            setStyleType(styleType);
        }

        // Style
        void setStyleType(const StyleType& styleType){
            switch (styleType) {
                case StyleType::Style2:
                    past_sign = "█";
                    current_sign = "▏▎▍▋▊▉█";
                    future_sign = " ";
                    break;
                case StyleType::Style3:
                    past_sign = "█";
                    current_sign = "█";
                    future_sign = "░";
                    break;
                case StyleType::Style4:
                    use_emoji=true;
                    use_emoji_sequence = true;
                    start_processing_emoji = "";
                    start_finish_emoji = "";
                    trail_finish_emoji = "✅";
                    break;
                default:
                    // Only mean that I have consider this
                    break;
            }
        }






    };

}


#endif //KIEPB_PROGRESSBAR_H
