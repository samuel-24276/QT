Ini文件读写、增删改查

- 2020-11-10    14：00

  Ini文件有三部分，节、键值对（参数）、注释（以；开头）。
  
  - 读——`QVector<QMap<QString,QString> > read();`
  - 写——`void write();`
  - 增加参数——`bool addParam(QString group, QString key, QString value);`
  - 删除参数——`bool deleteParam(QString group, QString key);`
  - 删除节——`deleteGroup(QString group);`
  - 更新参数值——`bool updateParam(QString group, QString key, QString value);`
  - 查找参数中键对应值——`QString findParam(QString group, QString key);`
  - 查找参数对应的节——`QString findParamGroup(QString key, QString value);`
  - 节中最大的id——`int maxGroupId();`