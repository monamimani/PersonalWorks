
#ifndef TESTSUITENAME_OP1
  #error "TESTSUITENAME_OP1 is not defined!"
#endif // !TESTSUITENAME

#ifndef TESTSUITENAME_OP2
  #error "TESTSUITENAME_OP2 is not defined!"
#endif // !TESTSUITENAME

TEST_P(TESTSUITENAME_OP1, CtorDtor)
{
  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);

    int value = 28;
    m_delegate.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
  }
}

TEST_P(TESTSUITENAME_OP1, CopyCtor)
{
  DelegateLike delegateCopy{m_delegate};

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegate);
    ASSERT_TRUE((bool)delegateCopy);

    int value = 28;
    m_delegate.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));

    value = 28;
    delegateCopy.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegate);
    ASSERT_FALSE((bool)delegateCopy);
  }

  ASSERT_EQ(m_delegate, delegateCopy);
}

TEST_P(TESTSUITENAME_OP1, MoveCtor)
{
  DelegateLike delegateMove{std::move(m_delegate)};

  if (!m_isBindKindEmpty)
  {
    ASSERT_EQ((bool)m_delegate, ValidStateTraits<DelegateLike>::m_IsMovedFromValid); // This really is a valid unspecified state, because moved from
    //ASSERT_FALSE((bool)m_delegate); // This really is a valid unspecified state, because moved from
    ASSERT_TRUE((bool)delegateMove);

    int value = 28;
    delegateMove.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    //ASSERT_EQ((bool)m_delegate, ValidStateTraits<DelegateLike>::m_IsMovedFromValid); // This really is a valid unspecified state, because moved from
    ASSERT_FALSE((bool)m_delegate); // This really is a valid unspecified state, because moved from
    ASSERT_FALSE((bool)delegateMove);
  }
}

TEST_P(TESTSUITENAME_OP1, operatorEqual)
{

  DelegateLike delegateCopy = m_delegate;

  if (!m_isBindKindEmpty)
  {
    ASSERT_TRUE(delegateCopy);
  }
  else
  {
    ASSERT_FALSE(delegateCopy);
  }
  ASSERT_EQ(m_delegate, delegateCopy);

  DelegateLike delegateTmp;
  if (!m_isBindKindEmpty)
  {
    ASSERT_NE(m_delegate, delegateTmp);
  }
  else
  {
    ASSERT_EQ(m_delegate, delegateTmp);
  }

  ASSERT_EQ(delegateTmp, delegateTmp);
}

TEST_P(TESTSUITENAME_OP1, invoke)
{

  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    if constexpr (std::is_same_v<DelegateLike, Delegate::Delegate<FctSignature>> || std::is_same_v<DelegateLike, Delegate::DelegateMulticast<FctSignature>>)
    {
      m_delegate.invoke(value);
      ASSERT_EQ(value, 28);
    }
  }
}

TEST_P(TESTSUITENAME_OP1, functionalOperator)
{

  int value = 28;
  if (!m_isBindKindEmpty)
  {
    m_delegate(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isConst));
  }
  else
  {
    if constexpr (std::is_same_v<DelegateLike, Delegate::Delegate<FctSignature>> || std::is_same_v<DelegateLike, Delegate::DelegateMulticast<FctSignature>>)
    {
      m_delegate.invoke(value);
      ASSERT_EQ(value, 28);
    }
  }
}

TEST_P(TESTSUITENAME_OP2, CopyAssign)
{
  m_delegateB = m_delegateA;

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA);
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));

    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA);
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, m_delegateB);
}

TEST_P(TESTSUITENAME_OP2, MoveAssign)
{
  m_delegateB = std::move(m_delegateA);

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_EQ((bool)m_delegateA, ValidStateTraits<DelegateLike>::m_IsMovedFromValid); // This really is a valid unspecified state, because moved from
    //ASSERT_FALSE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_TRUE((bool)m_delegateB);

    int value = 28;
    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA); // This realy is a valid unspecified state, becaused moved from
    ASSERT_FALSE((bool)m_delegateB);
    ASSERT_EQ(m_delegateA, m_delegateB); // This realy is a valid unspecified state, becaused moved from
  }
}

TEST_P(TESTSUITENAME_OP2, Swap)
{
  DelegateLike delegateACopy = m_delegateA;
  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)delegateACopy);
  }
  else
  {
    ASSERT_FALSE((bool)delegateACopy);
  }

  DelegateLike delegateBCopy = m_delegateB;
  if (!m_isDelegateBBindKindEmpty)
  {
    ASSERT_TRUE((bool)delegateBCopy);
  }
  else
  {
    ASSERT_FALSE((bool)delegateBCopy);
  }

  using std::swap;
  swap(m_delegateA, m_delegateB);

  if (!m_isDelegateBBindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateA);

    int value = 28;
    m_delegateA.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateBConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateA);
  }

  if (!m_isDelegateABindKindEmpty)
  {
    ASSERT_TRUE((bool)m_delegateB);
    int value = 28;
    m_delegateB.invoke(value);
    ASSERT_EQ(value, getExpectedTestValue(m_isDelegateAConst));
  }
  else
  {
    ASSERT_FALSE((bool)m_delegateB);
  }

  ASSERT_EQ(m_delegateA, delegateBCopy);
  ASSERT_EQ(m_delegateB, delegateACopy);
}
