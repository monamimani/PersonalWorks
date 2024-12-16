module;

export module CoreSingleton;

export namespace Core
{

template <typename T>
class Singleton
{
private:
public:
  static T& getInstance()
  {
    return m_instance;
  }

private:
  inline static constinit T m_instance = {};
};

template <typename T>
class SingletonLazy
{
private:
public:
  static T& getInstance()
  {
    static constinit T instance = {};
    return instance;
  }

private:
};

} // namespace Core
