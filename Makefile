# Markdown -> HTML Converter (OOP Project)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc
SRC = src/main.cpp \
      src/Utils.cpp \
      src/TreePrinter.cpp \
      src/InlineNode.cpp \
      src/InlineParser.cpp \
      src/Block.cpp \
      src/BlockParser.cpp \
      src/MarkdownDocument.cpp \
      src/Command.cpp \
      src/CommandLine.cpp
TARGET = markdown_converter

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(TARGET).exe examples/output.html

run: $(TARGET)
	./$(TARGET)
