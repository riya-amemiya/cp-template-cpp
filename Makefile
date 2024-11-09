# コンパイラとフラグ
CXX = g++
CXXFLAGS = -Wall -g -std=c++20
INCDIR = ./include

# ターゲットとソースファイル
TARGET = main.out
SRCDIR = ./src
OBJDIR = ./obj
OUTDIR = ./dist
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# ビルドルール
$(OUTDIR)/$(TARGET): $(OBJS) | $(OUTDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c -o $@ $<

# ディレクトリ作成ルール
$(OUTDIR) $(OBJDIR):
	mkdir -p $@

# ファイル削除ルール
clean:
	rm -rf $(OBJDIR) $(OUTDIR)/$(TARGET)

# ファイル依存関係を含める
-include $(OBJS:.o=.d)

# .cppファイルから.dファイルを生成するルール
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -MM -MP -MT '$(OBJDIR)/$*.o' -MF $@ $<