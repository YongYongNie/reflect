# reflect
c++实现 反射 reflect

测试 

class CIIII : public IIReflectBase {
    REF_DECLARE_DYNCREATE(CIIII);
  public:
  protected:
  private:
};

REF_IMPLEMENT_DYNCREATE(CIIII, L"CIII", 1, 1);

int main(int argc, char *argv[]) {
    
    std::unique_ptr<IIReflectBase> pBase(REF_RUNTIME_DYNCREATE(CIIII, L"CIII"));

    return 0;
}
