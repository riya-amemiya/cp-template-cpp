// 入出力の高速化
struct FastStdinBuf : std::streambuf {
  static constexpr size_t SZ = 1 << 20;
  std::vector<char> buf;
  FastStdinBuf() : buf(SZ) { setg(buf.data(), buf.data(), buf.data()); }
  int underflow() override {
    size_t nread = std::fread(buf.data(), 1, SZ, stdin);
    if (nread == 0)
      return traits_type::eof();
    setg(buf.data(), buf.data(), buf.data() + nread);
    return traits_type::to_int_type(*gptr());
  }
};
void fastio() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  static FastStdinBuf inbuf;
  cin.rdbuf(&inbuf);
  cout << fixed << setprecision(20);
}
