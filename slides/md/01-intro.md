layout: true
class: left, middle

---
background-image: url(graphics/MCCWA.png)
background-size: contain
---
# Modern C++ for the C-Wielding Aspirant
## Introduction
Josh Lospinoso
---

Target Audience
==
This course is designed for you if

1. you want to learn modern C++, and
2. you understand this program:

```c
int fn(int *y, int z) {
	int x = 0;
	for (int i = 0; i<z; i++) {
		if (y[i])
			x++;
	}
	return x;
}

int main() {
	int n = 10;
	int *x = (int*)malloc(n * sizeof(int));
	memset(x, 0, n * sizeof(int));
	for (int i = 1; i<n; i++) {
		x[i] = i % 2;
	}
	printf("%u", fn(x, n));
	free(x);
}
```

---

> In C++ it's harder to shoot yourself in the foot, but when you do, you blow off your whole leg.
> > Bjarne Stroustrup

---

Some Expectation Management
==
C++ is a very, _very_ complicated language.

You are going to learn some essentials. In the next two weeks, I hope for two
things:

1. that you are enabled to learn more C++, and
2. that you will be more useful in the workplace.

---


```cpp
cout << &josh << endl;
```

* `josh@lospi.net`
* PGP fingerprint `0xDE82E75CEEB683C4`
* I keep a blog at [lospi.net](https://lospi.net)

---

# Course Plan
* *Basics* (2 days)
* *Standard Template Library* (3 days)
* *Resource Management* (2 days)
* *Crucible/Test* (3 days)

---

# Test Driven Demonstrations

Lessons are interspersed with live demonstrations.

These demonstrations consist of a set of _tests_ and some partially implemented
_production code_. We will fill in the _production code_ to make the tests pass.

This is how you should write code in real life. It is called *test driven development* (TDD)
and it has many benefits.

---

# Getting set up for the course

1. Give me your email address so I can invite you to the class Slack channel.
2. Sign up for an _ngrok_ account. See https://jlospinoso.github.io/posts.html >> "Setting Up a Matterbot...".
3. Send me a private message on Slack with (a) your _ngrok_ url and (b) a name for your bot. I will give you a
hook URL and a token.
4. `git clone` the Matterbot project, input your hook url/token, and post a message with your bot.
5. If you don't already have a Gitlab account, get one (https://gitlab.com).
6. Send me your gitlab account ID on Slack.
