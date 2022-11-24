module;

export module CoreSingleton;

export namespace Core
{

template <typename T>
class Singleton
{
private:
public:
  T& getInstance() const
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
  T& getInstance() const
  {
    inline static constinit T instance = {};
    return instance;
  }

private:
};

} // namespace Core