### org.apache.maven:maven-artifact

org.apache.maven.artifact.versioning.DefaultArtifactVersion是Maven中用于表示依赖项版本的类，它提供了一些方法来比较和操作版本号。

默认情况下，版本号被解析为三个部分：主版本号，次版本号和修订版本号。例如，版本号1.2.3将被解析为主版本号1，次版本号2和修订版本号3。

DefaultArtifactVersion类提供了一些方法来比较版本号，例如：

compareTo(DefaultArtifactVersion o)：比较两个版本号的大小，如果当前版本号大于参数版本号，则返回正整数；如果当前版本号小于参数版本号，则返回负整数；如果两个版本号相等，则返回0。
equals(Object obj)：比较当前版本号是否与指定对象相等。
getMajorVersion()：获取主版本号。
getMinorVersion()：获取次版本号。
getIncrementalVersion()：获取修订版本号。
例如，以下代码比较两个版本号并打印结果：
```groovy
def v1 = new DefaultArtifactVersion("1.2.3")
def v2 = new DefaultArtifactVersion("1.2.4")
if (v1.compareTo(v2) < 0) {
println "${v1} is less than ${v2}"
} else if (v1.compareTo(v2) > 0) {
println "${v1} is greater than ${v2}"
} else {
println "${v1} is equal to ${v2}"
}
```
此代码将输出1.2.3 is less than 1.2.4，因为v1版本号小于v2版本号。

请注意，DefaultArtifactVersion类还支持解析和比较带有限定符的版本号，例如1.2.3-alpha1和1.2.3-beta2。这些版本号将被解析为主版本号1，次版本号2，修订版本号3和限定符alpha1或beta2。

### 
```groovy
def findCMake(filter) {
    def out = "cmake --version".execute().text.split("\n")[0].split(" ").last()
    println "found cmake:" + out
    def found = new DefaultArtifactVersion(out);
    def required = new DefaultArtifactVersion(filter);
    if (found > required)
        return out;
    return filter
}
```