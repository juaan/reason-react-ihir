open Utils
requireCSS("./BlogComponent.css");

type blogPost = {
  id: int,
  title: string,
  highlight: string,
  datePosted: string,
};

let myList: list(blogPost) = [
  {
    id: 1,
    title: "Post Pertama Urang",
    highlight: "hari ini seneng banget deh bisa ngepost blog pertama kali...",
    datePosted: "kemarin"
  }, 
  {
    id: 2,
    title: "Reason ML bukan Mobile Legend",
    highlight: "Reason ML adalah programmign language baru yang di develop oleh...",
    datePosted: "2 hari yang lalu"
  },
];


module BlogItem = {
  [@react.component]
  let make = (~post) => {
    <div className="blog-list">
        <div className="header-title-container">
            <h3 className="header-title">{React.string(post.title)}</h3>
            <p className="date">{React.string(post.datePosted)}</p>
        </div>
        <p>{React.string(post.highlight)}</p>
    </div>
  }
}


type newPost = {
  title: string,
  highlight: string,
  datePosted: string,
}

let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;

module Input = {
  type state = newPost;
  [@react.component]
  let make = (~onSubmit) => {
    let initialPost = {
      title: "",
      highlight: "",
      datePosted: "",
    };
    let (post, setPost) = React.useReducer((_, newPost) => {
      newPost
    }, initialPost);

    <div>
      <input 
        value=post.title
        type_="text"
        placeholder="title"
        onChange=((evt) => setPost({
          title: valueFromEvent(evt),
          highlight: post.highlight,
          datePosted: post.datePosted,
        }))
      />
      <input 
        value=post.highlight
        type_="text"
        placeholder="highlight"
        onChange=((evt) => setPost({
          title: post.title,
          highlight: valueFromEvent(evt),
          datePosted: post.datePosted,
        }))
      />
      <input 
        value=post.datePosted
        type_="text"
        placeholder="datePosted"
        onChange=((evt) => setPost({
          title: post.title,
          highlight: post.highlight,
          datePosted: valueFromEvent(evt),
        }))
      />
      <button onClick={(_evt) => {
        onSubmit(post)
        setPost(initialPost)
      }}>
        {"Add New Post" |> React.string}
      </button>
    </div>
  }
}

type action = 
  | AddPost(newPost);

type state = {
  posts: list(blogPost),
};

let addNewPost = (newPost, lastId) => {id: lastId + 1, title: newPost.title, highlight: newPost.highlight, datePosted: newPost.datePosted};

[@react.component]
let make = () => {
    let ({posts}, dispatch) = React.useReducer((state, action) => {
      let lastBlogPost = List.nth(state.posts, List.length(state.posts) - 1);
      switch action {
      | AddPost(newPost) => {posts: [addNewPost(newPost, lastBlogPost.id), ...state.posts]}
      };
    }, {
      posts: myList,
    });


    let blogPost = List.map((blog) => {
      <BlogItem post={blog} key={blog.id |> string_of_int} />;
    }, posts);

    let totalPost = List.length(posts);
    <div className="container">
        <h1>{"Blog Post Urang" |> React.string}</h1>
        <Input onSubmit={(post) => dispatch(AddPost(post))} />
        {blogPost |> Array.of_list |> ReasonReact.array}
        <div>
          {React.string(string_of_int(totalPost) ++ " Posts")}
        </div>
    </div>
}


